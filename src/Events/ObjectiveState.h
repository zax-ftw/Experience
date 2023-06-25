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
	using State = RE::QUEST_OBJECTIVE_STATE;

	static bool IsCompleted(State state);
};

