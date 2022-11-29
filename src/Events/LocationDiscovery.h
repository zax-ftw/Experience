#pragma once

#include "Experience.h"

class LocationDiscoveryEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::LocationDiscovery::Event>
{
public:
	using LocationDiscoveryEventSource = RE::BSTEventSource<RE::LocationDiscovery::Event>;

	LocationDiscoveryEventHandler(ExperienceManager* manager);
	~LocationDiscoveryEventHandler(void);

	RE::BSEventNotifyControl ProcessEvent(const RE::LocationDiscovery::Event* event, LocationDiscoveryEventSource* source) override;

private:
	using MarkerType = RE::MARKER_TYPE;

	int GetReward(MarkerType type);
};
