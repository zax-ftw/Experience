#include "Hooks/Actor.h"
#include "Offsets.h"
#include "Settings.h"
#include "Utils/GameSettings.h"

using namespace RE;

bool ActorEx::IsPlayerOrTeam(Actor* actor)
{
	if (actor->GetPlayerControls()) {
		return true;
	}

	if (actor->IsPlayerTeammate()) {
		return true;
	}

	if (auto cmdr = actor->GetCommandingActor()) {
		if (IsPlayerOrTeam(cmdr.get())) {
			return true;
		}
	}
	return false;
}

// Actor::DamageHealth
void ActorEx::ModTrackedDamage_Hook(AIProcess* process, Actor* attacker, float damage)
{
	auto player = PlayerCharacter::GetSingleton();
	auto target = process ? process->GetUserData() : nullptr;

	if ((attacker && IsPlayerOrTeam(attacker)) ||
		(target && target->IsCombatTarget(player))) {
		attacker = player;
	}
	_ModTrackedDamage(process, attacker, damage);
}

// Unknown::HandleAction
void ActorEx::KillMoveStart_Hook(Actor* victim, Actor* killer)
{
	float currentHealth = victim->GetActorValue(ActorValue::kHealth);
	if (currentHealth > 0.0f) {
		ModTrackedDamage_Hook(victim->currentProcess, killer, currentHealth);
	}
	_KillMoveStart(victim, killer);
}

void ActorEx::Install(SKSE::Trampoline& trampoline)
{
	_ModTrackedDamage = trampoline.write_call<5>(
		Offset::Actor::DamageHealth.address() + OFFSET(0xFD, 0x131, 0xFD), ModTrackedDamage_Hook);

	_KillMoveStart = trampoline.write_call<5>(
		Offset::Unknown::HandleAction.address() + OFFSET(0x1A7, 0x1D1, 0x1A7), KillMoveStart_Hook);
}
