#pragma once

#include "Experience.h"

class ObjectiveStateEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::ObjectiveState::Event>
{
public:
	using ObjectiveStateEventSource = RE::BSTEventSource<RE::ObjectiveState::Event>;

	ObjectiveStateEventHandler(ExperienceManager* manager);
	~ObjectiveStateEventHandler() override;

	RE::BSEventNotifyControl ProcessEvent(const RE::ObjectiveState::Event* event, ObjectiveStateEventSource* source) override;

private:
	using ObjectiveState = RE::QUEST_OBJECTIVE_STATE;

	static std::string GetFormattedDisplayText(RE::BGSQuestObjective* objective);
	int GetReward(ObjectiveState oldState, ObjectiveState newState);
};
