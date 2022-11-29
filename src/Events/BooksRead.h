#pragma once

#include "Skyrim/BookRead.h"

#include "Experience.h"

class BookReadEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::BookRead::Event>
{
public:
	using BookReadEventSource = RE::BSTEventSource<RE::BookRead::Event>;

	BookReadEventHandler(ExperienceManager* manager);
	~BookReadEventHandler(void);

	RE::BSEventNotifyControl ProcessEvent(const RE::BookRead::Event* event, BookReadEventSource* source) override;

private:
	int GetReward(float value);

};

