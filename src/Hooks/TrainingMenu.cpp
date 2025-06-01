#include "TrainingMenu.h"

#include "Hooks/PlayerSkills.h"
#include "Utils/GameSettings.h"
#include "Offsets.h"
#include "Settings.h"

using namespace RE;

void TrainingMenuEx::UpdateSkillMeter(bool a_targetPercent, bool a_skipPercentCalc)
{
	using func_t = decltype(&TrainingMenuEx::UpdateSkillMeter);
	REL::Relocation<func_t> func { Offset::TrainingMenu::UpdateSkillMeter };
	return func(this, a_targetPercent, a_skipPercentCalc);
}

void TrainingMenuEx::TrainSkill_Hook(TrainingMenuEx* menu)
{
	if (!PlayerSkillsEx::IsSkillCapped(menu->skill)) {
		_TrainSkill(menu);
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
	_TrainSkill = trampoline.write_branch<6>(
		Offset::TrainingMenu::Train.address() + OFFSET(0x7, 0x7, 0x7), TrainSkill_Hook);


}
