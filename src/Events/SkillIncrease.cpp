#include "Events/SkillIncrease.h"

#include "Hooks/PlayerSkills.h"
#include "Utils/GameSettings.h"

using namespace RE;

SkillIncreaseEventHandler::SkillIncreaseEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kActive)
{	
	SkillIncrease::GetEventSource()->AddEventSink(this);
}

SkillIncreaseEventHandler::~SkillIncreaseEventHandler()
{
	SkillIncrease::GetEventSource()->RemoveEventSink(this);
}

float SkillIncreaseEventHandler::GetExperienceGain(float skillLevel) const
{
	float mult = Utils::GetGameSettingFloat("fXPPerSkillRank");
	return skillLevel * mult;
}

BSEventNotifyControl SkillIncreaseEventHandler::ProcessEvent(const SkillIncrease::Event* event, SkillIncreaseEventSource*)
{
	float level = event->player->GetBaseActorValue(event->actorValue);

	logger::info("[SkillIncrease] {0} ({1})", event->actorValue, level);

	float reward = GetExperienceGain(level);
	AddExperience(reward);

	return BSEventNotifyControl::kContinue;
}
