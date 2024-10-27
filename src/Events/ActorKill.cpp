#include "Events/ActorKill.h"

#include <algorithm>
#include <math.h>

#include "Settings.h"
#include "Utils/DirectoryParser.h"
#include "Hooks/Actor.h"

using namespace RE;

ActorKillEventHandler::ActorKillEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kInactive)
{
	ActorKill::GetEventSource()->AddEventSink(this);

	Utils::ParseDirectory("Data/SKSE/Plugins/Experience/Actors", npcs);
	Utils::ParseDirectory("Data/SKSE/Plugins/Experience/Races", races);
}

ActorKillEventHandler::~ActorKillEventHandler()
{
	ActorKill::GetEventSource()->RemoveEventSink(this);
}

RE::BSEventNotifyControl ActorKillEventHandler::ProcessEvent(const RE::ActorKill::Event* event, ActorKillEventSource*)
{
	auto player = PlayerCharacter::GetSingleton();
	auto victim = event->victim;

	if (IsValidKill(victim, player)) {

		logger::info("[ActorKill] {} (RefID:{:08X})",
			victim->GetName(),
			victim->GetFormID());

		float reward = GetBaseReward(victim);

		reward *= GetLevelMult(victim, player);
		reward *= GetGroupMult(player);
		reward *= Settings::GetSingleton()->GetValue<float>("fKillingMult");

		int result = std::max(std::ceil(reward), 1.0f);

		AddExperience(result);
	}

	return BSEventNotifyControl::kContinue;
}

float ActorKillEventHandler::GetLevelMult(const Actor* victim, const Actor* killer)
{
	float setting = Settings::GetSingleton()->GetValue<float>("fLevelFactor");

	float levelFactor = std::clamp(setting, 0.0f, 1.0f);
	float levelRatio = (float)victim->GetLevel() / (float)killer->GetLevel();

	return std::pow(levelRatio, levelFactor);
}

float ActorKillEventHandler::GetGroupMult(const PlayerCharacter* player)
{
	float setting = Settings::GetSingleton()->GetValue<float>("fGroupFactor");

	float groupFactor = std::clamp(setting, 0.0f, 0.5f);
	float groupSize = player->teammateCount;

	return std::pow(1.0f - groupFactor, groupSize);
}

float ActorKillEventHandler::GetBaseReward(Actor* actor)
{
	auto base = actor->GetTemplateBase();
	if (auto it = npcs.find(base); it != npcs.end()) {
		return (float)it->second;
	}

	auto race = base->GetRace();
	if (auto it = races.find(race); it != races.end()) {
		return (float)it->second;
	}
	return 0.0f;
}

float ActorKillEventHandler::GetPlayerDamagePercent(Actor* actor)
{
	float totalHealth = actor->GetBaseActorValue(ActorValue::kHealth);
	float totalDamage = actor->GetTrackedDamage();

	return totalDamage / totalHealth;
}

bool ActorKillEventHandler::IsValidKill(Actor* victim, Actor* killer)
{
	if (victim->IsSummoned() || victim->IsReanimated()) {
		return false;
	}

	float damageThreshold = Settings::GetSingleton()->GetValue<float>("fDamageThreshold");

	if (GetPlayerDamagePercent(victim) < damageThreshold) {
		return false;
	}

	return true;
}
