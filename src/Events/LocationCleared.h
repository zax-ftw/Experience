#pragma once

#include "Experience.h"

class LocationClearedEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::LocationCleared::Event>
{
public:
	using LocationClearedEventSource = RE::BSTEventSource<RE::LocationCleared::Event>;

	LocationClearedEventHandler(ExperienceManager* manager);
	~LocationClearedEventHandler() override;

	RE::BSEventNotifyControl ProcessEvent(const RE::LocationCleared::Event* event, LocationClearedEventSource* source) override;

private:
	using MarkerType = RE::MARKER_TYPE;

	int  GetReward(MarkerType type);
	void ShowLocationCleared(const char* location);
};
