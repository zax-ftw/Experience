#pragma once

#include "Skyrim/QuestStatus.h"

#include "Experience.h"

class QuestStatusEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::QuestStatus::Event>
{
public:
	using QuestStatusEventSource = RE::BSTEventSource<RE::QuestStatus::Event>;

	QuestStatusEventHandler(ExperienceManager* manager);
	~QuestStatusEventHandler();

	RE::BSEventNotifyControl ProcessEvent(const RE::QuestStatus::Event* event, QuestStatusEventSource* source) override;

private:
	using QuestType = RE::QUEST_DATA::Type;

	int GetReward(QuestType type);
};
