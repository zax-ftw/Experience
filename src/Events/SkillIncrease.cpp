#include "Events/SkillIncrease.h"

#include "Hooks/PlayerSkills.h"

using namespace RE;

SkillIncreaseEventHandler::SkillIncreaseEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kActive)
{	
	SkillIncrease::GetEventSource()->AddEventSink(this);
}

SkillIncreaseEventHandler::~SkillIncreaseEventHandler(void)
{
	SkillIncrease::GetEventSource()->RemoveEventSink(this);
}

float SkillIncreaseEventHandler::GetExperienceGain(float skillLevel) const
{
	auto settings = GameSettingCollection::GetSingleton();
	Setting* setting = settings->GetSetting("fXPPerSkillRank");

	float fXPPerSkillRank = setting->GetFloat();

	return skillLevel * fXPPerSkillRank;
}

BSEventNotifyControl SkillIncreaseEventHandler::ProcessEvent(const SkillIncrease::Event* event, SkillIncreaseEventSource*)
{
	auto player = event->player->AsActorValueOwner();
	float level = player->GetBaseActorValue(event->actorValue);

	logger::info("SkillIncrease: {0} ({1})", event->actorValue, level);

	float reward = GetExperienceGain(level);
	AddExperience(reward);

	return BSEventNotifyControl::kContinue;
}
