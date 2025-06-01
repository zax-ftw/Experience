#pragma once

class TrainingMenuEx : public RE::TrainingMenu
{
public:

	static void Install(SKSE::Trampoline& trampoline);

private:

	static void TrainSkill_Hook(TrainingMenuEx* menu);
	static void ShowCappedMessage();

	static inline REL::Relocation<decltype(Train_Hook)> _Train;
	static inline REL::Relocation<decltype(TrainSkill_Hook)> _TrainSkill;
};
