#pragma once

class ActorEx : public RE::Actor
{
public:
	static void Install(SKSE::Trampoline& trampoline);
	static bool IsPlayerOrTeam(RE::Actor* actor);

private:
	static void ModTrackedDamage_Hook(RE::AIProcess* process, RE::Actor* attacker, float damage);
	static void StartKillMove_Hook(RE::Actor* victim, RE::Actor* killer);

	static inline REL::Relocation<decltype(ModTrackedDamage_Hook)> _ModTrackedDamage;
	static inline REL::Relocation<decltype(StartKillMove_Hook)> _StartKillMove;
};
