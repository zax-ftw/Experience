#pragma once

class BGSLocationEx : public RE::BGSLocation
{
public:
	static BGSLocationEx* GetLastChecked();
	static void           Install(SKSE::Trampoline& trampoline);

	RE::MARKER_TYPE GetMapMarkerType();

	bool CheckLocationCleared(int time, bool force);

private:
	static bool CheckLocationCleared_Hook(BGSLocationEx* location, int time, bool force);

	// members
	static BGSLocationEx* lastChecked;

	static inline REL::Relocation<decltype(CheckLocationCleared_Hook)> _CheckLocationCleared;
};
