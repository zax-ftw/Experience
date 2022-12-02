#include "Events/ActorKill.h"

#include <algorithm>

#include "Settings.h"

using namespace RE;

ActorKillEventHandler::ActorKillEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kInactive)
{
	ActorKill::GetEventSource()->AddEventSink(this);

	ParseDirectory("Data/SKSE/Plugins/Experience/Actors", npcs);
	ParseDirectory("Data/SKSE/Plugins/Experience/Races", races);
}

ActorKillEventHandler::~ActorKillEventHandler(void)
{
	ActorKill::GetEventSource()->RemoveEventSink(this);
}

void ActorKillEventHandler::ParseFile(const fs::path& file, std::unordered_map<TESForm*, int>& data)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	
	SI_Error rc = ini.LoadFile(file.c_str());
	if (rc < 0) {
		logger::warn("Failed to parse file: {}", file.filename().string());
		return;
	}

	logger::info("Parsing file: {}", file.filename().string());

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);

	TESDataHandler* dataHandler = TESDataHandler::GetSingleton();
	for (const auto& section : sections) {

		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys(section.pItem, keys);

		for (const auto& key : keys) {

			FormID formID = std::strtol(key.pItem, nullptr, 16);

			auto form = dataHandler->LookupForm(formID, section.pItem); 
			if (form) {

				int value = ini.GetLongValue(section.pItem, key.pItem);
				logger::trace("{0} | {1:08X} | {2}", section.pItem, formID, value);

				data[form] = value;
			}
		}
	}
}

void ActorKillEventHandler::ParseDirectory(const fs::path& root, std::unordered_map<TESForm*, int>& data)
{
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	using std::chrono::steady_clock;

	auto t1 = steady_clock::now();

	if (fs::exists(root) && fs::is_directory(root)) {
		for (auto& entry : fs::directory_iterator(root)) {
			if (entry.is_regular_file() && entry.path().extension() == ".ini") {
				ParseFile(entry.path(), data);
			}
		}
	}

	auto t2 = steady_clock::now();
	auto ms = duration_cast<milliseconds>(t2 - t1);

	logger::info("Directory '{}' parsed in {} ms", root.string() , ms.count());
}

BSEventNotifyControl ActorKillEventHandler::ProcessEvent(const ActorKill::Event* event, ActorKillEventSource*)
{
	if (IsValidKill(event->killer, event->victim)) {

		auto player = PlayerCharacter::GetSingleton();
		auto victim = event->victim;

		logger::info("ActorKill: {0} ({1})",
			victim->GetName(),	
			victim->GetRace()->GetFormEditorID());

		float reward = GetBaseReward(victim);

		reward *= GetLevelMult(player, victim);
		reward *= GetGroupMult(player);
		reward *= Settings::GetSingleton().GetSettingFloat("fXPKillingMult");

		int result = max(std::ceil(reward), 1.0f);

		AddExperience(result);
	}
	return BSEventNotifyControl::kContinue;
}

float ActorKillEventHandler::GetLevelMult(Actor* player, Actor* victim) const
{
	auto setting = Settings::GetSingleton().GetSettingInt("iXPLevelRange");

	float levelRange = max(setting, 1);
	float levelDelta = player->GetLevel() - victim->GetLevel();

	return max(1.0f - (levelDelta / levelRange), 0.0f);
}

float ActorKillEventHandler::GetGroupMult(PlayerCharacter* player) const
{
	auto setting = Settings::GetSingleton().GetSettingFloat("fXPGroupFactor");

	float groupFactor = std::clamp(setting, 0.0f, 1.0f);
	float groupSize = player->GetInfoRuntimeData().teammateCount;

	return std::pow((1.0f - groupFactor), groupSize);
}

TESNPC* ActorKillEventHandler::GetTemplateBase(Actor* actor) const
{
	auto LeveledCreature = actor->extraList.GetByType<ExtraLeveledCreature>();
	if (LeveledCreature) {
		return static_cast<TESNPC*>(LeveledCreature->templateBase);
	}
	return actor->GetActorBase();
}

float ActorKillEventHandler::GetBaseReward(Actor* actor) const
{
	auto base = GetTemplateBase(actor);
	if (auto it = npcs.find(base); it != npcs.end()) {
		return (float)it->second;
	}

	auto race = base->GetRace();
	if (auto it = races.find(race); it != races.end()) {
		return (float)it->second;
	}
	return 0.0;
}

bool ActorKillEventHandler::IsValidKill(Actor* killer, Actor* victim)
{
	if (killer) {

		if (victim->IsSummoned() || victim->IsChild() || victim->IsGuard()) {
			return false;
		}

		if (killer->IsPlayerRef() || killer->IsPlayerTeammate()) {
			return true;
		}

		// TODO: handle it better
		if (killer->AsActorValueOwner()->GetActorValue(ActorValue::kAggression) == 3) { 
			return true;
		}
	}
	return false;
}
