#pragma once

class BGSLocationEx : public RE::BGSLocation
{
public:
	static BGSLocationEx* GetLastChecked() { return lastChecked; };
	static void           Install(SKSE::Trampoline& trampoline);

	RE::MARKER_TYPE GetMapMarkerType();

	bool ClearedCheck(int time, bool force);

private:
	bool ClearedCheck_Hook(int time, bool force);

	// members
	static BGSLocationEx* lastChecked;
};
