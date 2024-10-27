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
	using QuestType = RE::QUEST_DATA::Type;

	static bool IsCompleted(RE::QUEST_OBJECTIVE_STATE state);
};

