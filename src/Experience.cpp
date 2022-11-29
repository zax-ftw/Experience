#include "Experience.h"

#include "Settings.h"
#include "Translation.h"
#include "HUD.h"

#include "Events/ActorKill.h"
#include "Events/BooksRead.h"
#include "Events/LocationCleared.h"
#include "Events/LocationDiscovery.h"
#include "Events/ObjectiveState.h"
#include "Events/QuestStatus.h"
#include "Events/SkillIncrease.h"

using namespace RE;

ExperienceManager::ExperienceManager()
{
	worker = std::thread(&ExperienceManager::Loop, this);

	auto player = PlayerCharacter::GetSingleton();
	data = player->GetPlayerRuntimeData().skills->data;
}

ExperienceManager::~ExperienceManager()
{
	running.store(false);

	wakeup.notify_one();
	worker.join();
}

void ExperienceManager::Init()
{
	events.emplace_back(new QuestStatusEventHandler(this));
	events.emplace_back(new LocationDiscoveryEventHandler(this));
	events.emplace_back(new LocationClearedEventHandler(this));
	events.emplace_back(new ObjectiveStateEventHandler(this));

	Settings& settings = Settings::GetSingleton();

	if (settings.GetSettingBool("bEnableKilling")) {
		events.emplace_back(new ActorKillEventHandler(this));
	}
	if (settings.GetSettingBool("bEnableReading")) {
		events.emplace_back(new BookReadEventHandler(this));
	}
	if (settings.GetSettingBool("bEnableSkillXP")) {
		events.emplace_back(new SkillIncreaseEventHandler(this));
	}

	logger::info("Experience sources initialized");
}

void ExperienceManager::Loop()
{
	std::unique_lock<std::mutex> lock(mtx);

	do {
		wakeup.wait(lock); // mutex unlocked until CV is notified

		if (!queue.empty()) {
			Process(lock);
		}

	} while (running.load());
}

void ExperienceManager::Process(std::unique_lock<std::mutex>& lock)
{
	wakeup.wait_until(lock, timeout, [this] {  // mutex unlocked
		return !running.load();
	});

	int points = 0; 
	int	widget = 0;

	while (!queue.empty()) {
		Experience& e = queue.front();
		queue.pop();

		points += e.points;
		widget |= e.widget;
	}

	AddExperience(points, widget);
}

void ExperienceManager::AddExperience(const Experience& xp)
{
	std::unique_lock<std::mutex> lock(mtx);

	timeout = clock_t::now() + std::chrono::seconds(1);  // reset timer

	queue.push(xp);

	wakeup.notify_one();
}

void ExperienceManager::AddExperience(int points, bool meter)
{
	float xp_old = data->xp;
	float xp_new = xp_old + points;
	float xp_max = data->levelThreshold;

	data->xp = xp_new;

	logger::info("Progress: {0}/{2} => {1}/{2}", 
		xp_old, xp_new, xp_max);

	Settings& settings = Settings::GetSingleton();

	if (meter || settings.GetSettingBool("bForceLvlMeter")) {
		ShowLevelMeter(xp_old / xp_max, xp_new / xp_max);
	}
	if (xp_new >= xp_max && xp_old < xp_max) {
		ShowLevelUpNotification();
	}
	if (settings.GetSettingBool("bShowMessages")) {
		ShowRewardMessage(points);
	}
}

void ExperienceManager::Source::AddExperience(int points)
{
	if (points != 0) {

		Experience experience;

		experience.points = points;
		experience.widget = meter;
		experience.source = this;

		logger::trace("{0:+d} XP", points);

		manager->AddExperience(experience);
	}
}

void ExperienceManager::ShowLevelMeter(float start, float end)
{
	PlayerCharacter* player = PlayerCharacter::GetSingleton();
	uint16_t level = player->GetLevel();

	HUD::ShowLevelMeter(level, start, end);
}

void ExperienceManager::ShowLevelUpNotification()
{
	using Type = HUD::MessageType;

	std::string message = Translation::Translate("$LEVEL UP");

	HUD::ShowNotification(message.c_str(), "", "UILevelUp", Type::kLocationDiscoverd);
}

void ExperienceManager::ShowRewardMessage(int points)
{
	std::string format = Settings::GetSingleton().GetSettingString("sMessageFormat");
	std::string result = fmt::format(fmt::runtime(format), points);

	DebugNotification(result.c_str(), nullptr);
}
