#pragma once

class TrainingMenuEx : public RE::TrainingMenu
{
public:
	void Train();

	static void Install(SKSE::Trampoline& trampoline);

private:
	void Train_Hook();
	void ShowCappedMessage();
};
