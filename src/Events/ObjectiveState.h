#pragma once

#include "Experience.h"

class ObjectiveStateEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::ObjectiveState::Event>
{
public:
	using ObjectiveStateEventSource = RE::BSTEventSource<RE::ObjectiveState::Event>;

	ObjectiveStateEventHandler(ExperienceManager* manager);
	~ObjectiveStateEventHandler(void);

	RE::BSEventNotifyControl ProcessEvent(const RE::ObjectiveState::Event* event, ObjectiveStateEventSource* source) override;

private:
	using ObjState = RE::QUEST_OBJECTIVE_STATE;
	using QuestType = RE::QUEST_DATA::Type;

	static bool IsCompleted(ObjState state);
};

