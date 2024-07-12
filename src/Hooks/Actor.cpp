#include "Hooks/Actor.h"
#include "Offsets.h"
#include "Settings.h"
#include "Utils/GameSettings.h"

using namespace RE;

void DamageHealth(Actor* a_actor, float a_damage, Actor* a_attacker, bool a_a4)
{
	using func_t = decltype(&DamageHealth);
	REL::Relocation<func_t> func { Offset::Actor::DamageHealth };
	return func(a_actor, a_damage, a_attacker, a_a4);
}

bool IsCombatTarget(Actor* a_actor, Actor* a_other)
{
	using func_t = decltype(&IsCombatTarget);
	REL::Relocation<func_t> func { Offset::Actor::IsCombatTarget };
	return func(a_actor, a_other);
}

bool GetPlayerControls(const Actor* actor)
{
	auto& ctrl = actor->movementController;
	return ctrl && ctrl->unk1C5 ? true : false;
}

float ActorEx::GetTrackedDamage(Actor* actor)
{
	auto process = actor->currentProcess;
	return process ? process->trackedDamage : 0.0f;
}

void ActorEx::ModTrackedDamage(AIProcess* process, Actor* attacker, float damage)
{
	auto player = PlayerCharacter::GetSingleton();
	auto target = process->GetUserData();

	if ((attacker && IsInPlayerTeam(attacker)) || 
		(target && IsCombatTarget(target, player))) {
		attacker = player;
	}
	_ModTrackedDamage(process, attacker, damage);
}

bool ActorEx::IsInPlayerTeam(Actor* actor)
{
	if (GetPlayerControls(actor)) {
		//logger::trace("{} is controlled by player", actor->GetName());
		return true;
	}

	if (actor->IsPlayerTeammate()) {
		//logger::trace("{} is player teammate", actor->GetName());
		return true;
	}

	if (auto cmdr = actor->GetCommandingActor()) {
		//logger::trace("{} is commanded by {}", actor->GetName(), cmdr->GetName());
		if (IsInPlayerTeam(cmdr.get())) {
			return true;
		}
	}
	return false;
}

// Actor::DamageHealth
void ActorEx::ModTrackedDamage_Hook(AIProcess* process, Actor* attacker, float damage)
{
	ModTrackedDamage(process, attacker, damage);
}

// Unknown::HandleAction
void ActorEx::KillMoveStart_Hook(Actor* victim, Actor* killer)
{
	float currentHealth = victim->GetActorValue(ActorValue::kHealth);
	if (currentHealth > 0.0f) {
		ModTrackedDamage(victim->currentProcess, killer, currentHealth);
	}
	_KillMoveStart(victim, killer);
}

void ActorEx::Install(SKSE::Trampoline& trampoline)
{
	_ModTrackedDamage = trampoline.write_call<5>(
		Offset::Actor::DamageHealth.address() + OFFSET(0xFD, 0x131, 0xFD), &ActorEx::ModTrackedDamage_Hook);

	_KillMoveStart = trampoline.write_call<5>(
		Offset::Unknown::HandleAction.address() + OFFSET(0x1A7, 0x1D1, 0x1A7), &ActorEx::KillMoveStart_Hook);
}
