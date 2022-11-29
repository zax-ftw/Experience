#pragma once

#include "Experience.h"

namespace fs = std::filesystem;

class ActorKillEventHandler : public ExperienceManager::Source, 
	public RE::BSTEventSink<RE::ActorKill::Event>
{
public:
	using ActorKillEventSource = RE::BSTEventSource<RE::ActorKill::Event>;

	ActorKillEventHandler(ExperienceManager* manager);
	~ActorKillEventHandler();

	RE::BSEventNotifyControl ProcessEvent(const RE::ActorKill::Event* event, ActorKillEventSource* source) override;

	RE::TESNPC* GetTemplateBase(RE::Actor* actor) const;

	float GetBaseReward(RE::Actor* actor) const;
	float GetLevelMult(RE::Actor* player, RE::Actor* victim) const;
	float GetGroupMult(RE::PlayerCharacter* player) const;

	bool IsValidKill( RE::Actor* killer, RE::Actor* victim);

private:
	void ParseDirectory(const fs::path& root, std::unordered_map<RE::TESForm*, int>& data);
	void ParseFile(const fs::path& file, std::unordered_map<RE::TESForm*, int>& data);

	std::unordered_map<RE::TESForm*, int> races;
	std::unordered_map<RE::TESForm*, int> npcs;
};
