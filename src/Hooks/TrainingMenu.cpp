#include "TrainingMenu.h"

#include "Hooks/PlayerSkills.h"
#include "Utils/GameSettings.h"
#include "Offsets.h"
#include "Settings.h"

using namespace RE;

void TrainingMenuEx::Train()
{
	using func_t = decltype(&TrainingMenuEx::Train);
	REL::Relocation<func_t> func{ Offset::TrainingMenu::Train };
	return func(this);
}

void TrainingMenuEx::Train_Hook()
{
	logger::trace("Train_Hook: {0} (trainer={1:08X})", skill, trainer->formID);

	auto player = PlayerCharacter::GetSingleton();
	if (player->GetBaseActorValue(skill) < PlayerSkillsEx::GetSkillCap1(skill)) {
		Train();
	} else {
		ShowCappedMessage();
	}
}
void TrainingMenuEx::ShowCappedMessage()
{
	auto message = Utils::GetGameSettingString("sCanNotTrainAnymore");
	DebugNotification(message);
}

void TrainingMenuEx::Install(SKSE::Trampoline& trampoline)
{
	trampoline.write_branch<6>(
		Offset::TrainingMenu::TrainCallback::Accept.address() + OFFSET(0x7, 0x7, 0x7),
		&TrainingMenuEx::Train_Hook);

	trampoline.write_call<5>(
		Offset::TrainingMenu::ProcessMessage.address() + OFFSET(0xDB, 0xD8, 0xDB),
		&TrainingMenuEx::Train_Hook);
}
