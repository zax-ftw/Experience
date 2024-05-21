#include "Events/ActorKill.h"

#include <algorithm>

#include "Settings.h"

using namespace RE;

ActorKillEventHandler::ActorKillEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kInactive)
{
	ScriptEventSourceHolder::GetSingleton()->AddEventSink<TESDeathEvent>(this);

	ParseDirectory("Data/SKSE/Plugins/Experience/Actors", npcs);
	ParseDirectory("Data/SKSE/Plugins/Experience/Races", races);
}

ActorKillEventHandler::~ActorKillEventHandler(void)
{
	ScriptEventSourceHolder::GetSingleton()->RemoveEventSink<TESDeathEvent>(this);
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

RE::BSEventNotifyControl ActorKillEventHandler::ProcessEvent(const RE::TESDeathEvent* event, RE::BSTEventSource<RE::TESDeathEvent>*)
{
	Actor* victim = event->actorDying ? event->actorDying->As<Actor>() : nullptr;
	Actor* killer = event->actorKiller ? event->actorKiller->As<Actor>() : nullptr;

	if (event->dead) {
		HandleKill(victim, killer);
	}

	return BSEventNotifyControl::kContinue;
}

void ActorKillEventHandler::HandleKill(Actor* victim, Actor* killer)
{
	auto player = PlayerCharacter::GetSingleton();

	if (IsValidKill(victim, player)) {

		logger::info("[ActorDeath] {} (RefID:{:08X})",
			victim->GetName(),
			victim->GetFormID());

		float reward = GetBaseReward(victim);

		reward *= GetLevelMult(victim, player);
		reward *= GetGroupMult(player);
		reward *= Settings::GetSingleton()->GetValue<float>("fXPKillingMult");

		int result = std::max(std::ceil(reward), 1.0f);

		AddExperience(result);
	}
}

float ActorKillEventHandler::GetLevelMult(const Actor* victim, const Actor* killer)
{
	float setting = Settings::GetSingleton()->GetValue<float>("fXPLevelFactor");

	float levelFactor = std::clamp(setting, 0.0f, 1.0f);
	float levelRatio = (float)victim->GetLevel() / (float)killer->GetLevel();

	return std::pow(levelRatio, levelFactor);
}

float ActorKillEventHandler::GetGroupMult(const PlayerCharacter* player)
{
	float setting = Settings::GetSingleton()->GetValue<float>("fXPGroupFactor");

	float groupFactor = std::clamp(setting, 0.0f, 0.5f);
	float groupSize = player->GetInfoRuntimeData().teammateCount;

	return std::pow(1.0f - groupFactor, groupSize);
}

TESNPC* ActorKillEventHandler::GetTemplateBase(Actor* actor)
{
	auto LeveledCreature = actor->extraList.GetByType<ExtraLeveledCreature>();
	if (LeveledCreature) {
		return static_cast<TESNPC*>(LeveledCreature->templateBase);
	}
	return actor->GetActorBase();
}

float ActorKillEventHandler::GetBaseReward(Actor* actor)
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

bool ActorKillEventHandler::IsValidKill(Actor* victim, Actor* killer)
{
	if (victim->IsSummoned() || victim->IsCommandedActor()) {
		return false;
	}

	if (victim->IsChild() || victim->IsGuard()) {
		return false;
	}

	if (!victim->IsHostileToActor(killer)) {
		return false;
	}

	return true;
}
