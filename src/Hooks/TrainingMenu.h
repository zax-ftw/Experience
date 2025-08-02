#pragma once

class TrainingMenuEx : public RE::TrainingMenu
{
public:

	void UpdateSkillMeter(bool a_targetPercent, bool a_skipPercentCalc);

	static void Install(SKSE::Trampoline& trampoline);

private:

	template <int N>
	static void TrainSkill_Hook(TrainingMenuEx* menu);
	static void UpdateDisplay_Hook(TrainingMenuEx* menu);

	static void ShowCappedMessage();

	// hooked
	static inline REL::Relocation<decltype(TrainSkill_Hook<0>)> _TrainSkill[2];
	static inline REL::Relocation<decltype(UpdateDisplay_Hook)> _UpdateDisplay;
};
