#include "TrainingMenu.h"

#include "Hooks/PlayerSkills.h"
#include "Utils/GameSettings.h"
#include "Offsets.h"
#include "Settings.h"

using namespace RE;

void TrainingMenuEx::Train_Hook(TrainingMenuEx* menu)
{
	logger::trace("Train_Hook: {}", EnumToString(menu->skill));

	auto player = PlayerCharacter::GetSingleton();
	if (player->GetBaseActorValue(menu->skill) < PlayerSkillsEx::GetSkillCap1(menu->skill)) {
		_Train(menu);
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
	_Train = trampoline.write_branch<6>(
		Offset::TrainingMenu::TrainCallback::Accept.address() + OFFSET(0x7, 0x7, 0x7), Train_Hook);

	trampoline.write_call<5>(
		Offset::TrainingMenu::ProcessMessage.address() + OFFSET(0xDB, 0xD8, 0xDB), Train_Hook);
}
