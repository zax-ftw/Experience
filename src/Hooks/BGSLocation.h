#pragma once

class BGSLocationEx : public RE::BGSLocation
{
public:
	static BGSLocationEx* GetLastChecked() { return lastChecked; };
	static void           Install(SKSE::Trampoline& trampoline);

	RE::MARKER_TYPE GetMapMarkerType();

	bool ClearedCheck(int time, bool force);

private:
	static bool ClearedCheck_Hook(BGSLocationEx* location, int time, bool force);

	// members
	static BGSLocationEx* lastChecked;

	static inline REL::Relocation<decltype(ClearedCheck_Hook)> _ClearedCheck;
};
