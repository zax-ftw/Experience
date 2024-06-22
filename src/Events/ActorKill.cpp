#include "Events/ActorKill.h"

#include <algorithm>

#include "Settings.h"
#include "Utils/DirectoryParser.h"

using namespace RE;

ActorKillEventHandler::ActorKillEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kInactive)
{
	ScriptEventSourceHolder::GetSingleton()->AddEventSink<TESDeathEvent>(this);

	Utils::ParseDirectory("Data/SKSE/Plugins/Experience/Actors", npcs);
	Utils::ParseDirectory("Data/SKSE/Plugins/Experience/Races", races);
}

ActorKillEventHandler::~ActorKillEventHandler(void)
{
	ScriptEventSourceHolder::GetSingleton()->RemoveEventSink<TESDeathEvent>(this);
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
