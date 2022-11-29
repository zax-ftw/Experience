#include "TrainingMenu.h"

#include "Hooks/PlayerSkills.h"
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
	auto trainer = static_cast<Character*>(unk38);

	logger::trace("Train_Hook: {0} (trainer={1:08X})", skill, trainer->formID);

	auto player = PlayerCharacter::GetSingleton()->AsActorValueOwner();
	if (player->GetBaseActorValue(skill) < PlayerSkillsEx::GetSkillCap1(skill)) {
		Train();
	} else {
		ShowCappedMessage();
	}
}

void TrainingMenuEx::ShowCappedMessage()
{
	auto settings = GameSettingCollection::GetSingleton();
	Setting* setting = settings->GetSetting("sCanNotTrainAnymore");
	if (setting) {
		const char* message = setting->GetString();
		DebugNotification(message, nullptr);		
	}
}

void TrainingMenuEx::Install(SKSE::Trampoline& trampoline)
{
	trampoline.write_branch<6>(
		Offset::TrainingMenu::TrainCallback::Accept.address() + 0x7,
		&TrainingMenuEx::Train_Hook);

	trampoline.write_call<5>(
		Offset::TrainingMenu::ProcessMessage.address() + 0xDB,
		&TrainingMenuEx::Train_Hook);
}
