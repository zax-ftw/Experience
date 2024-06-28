#pragma once

class ActorEx : public RE::Actor
{
public:
	static void Install(SKSE::Trampoline& trampoline);

	static float GetTrackedDamage(RE::Actor* actor);
	static void ModTrackedDamage(RE::AIProcess* process, RE::Actor* attacker, float damage);
	static bool IsInPlayerTeam(RE::Actor* actor);

private:
	static void ModTrackedDamage_Hook(RE::AIProcess* process, RE::Actor* attacker, float damage);
	static void KillMoveStart_Hook(RE::Actor* victim, RE::Actor* killer);

	static inline REL::Relocation<decltype(ModTrackedDamage_Hook)> _ModTrackedDamage;
	static inline REL::Relocation<decltype(KillMoveStart_Hook)> _KillMoveStart;
};
