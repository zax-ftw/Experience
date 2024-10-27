#pragma once

#include "Experience.h"

class SkillIncreaseEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::SkillIncrease::Event>
{
public:
	using SkillIncreaseEventSource = RE::BSTEventSource<RE::SkillIncrease::Event>;

	SkillIncreaseEventHandler(ExperienceManager* manager);
	~SkillIncreaseEventHandler() override;

	RE::BSEventNotifyControl ProcessEvent(const RE::SkillIncrease::Event* event, SkillIncreaseEventSource* source) override;

private:
	float GetExperienceGain(float skillLevel) const;
};
