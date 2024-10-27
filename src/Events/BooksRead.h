#pragma once

#include "Experience.h"

class BooksReadEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::BooksRead::Event>
{
public:
	using BooksReadEventSource = RE::BSTEventSource<RE::BooksRead::Event>;

	BooksReadEventHandler(ExperienceManager* manager);
	~BooksReadEventHandler() override;

	RE::BSEventNotifyControl ProcessEvent(const RE::BooksRead::Event* event, BooksReadEventSource* source) override;

private:
	int GetReward(float value);

};

