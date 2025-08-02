#pragma once

class BGSLocationEx : public RE::BGSLocation
{
public:
	static BGSLocationEx* GetLastChecked();
	static void           Install(SKSE::Trampoline& trampoline);

	RE::MARKER_TYPE GetMapMarkerType();

private:

	template <int N>
	static bool CheckCleared_Hook(BGSLocationEx* location, int time, bool force);

	// members
	static inline BGSLocationEx* lastChecked = nullptr;

	static inline REL::Relocation<decltype(CheckCleared_Hook<0>)> _CheckCleared[4];
};
