#include "Hooks/BGSLocation.h"

#include "Offsets.h"

using namespace RE;

BGSLocationEx* BGSLocationEx::lastChecked = nullptr;

bool BGSLocationEx::CheckLocationCleared(int time, bool force)
{
	using func_t = decltype(&BGSLocationEx::CheckLocationCleared);
	REL::Relocation<func_t> func{ Offset::BGSLocation::CheckLocationCleared };
	return func(this, time, force);
}
// TODO: refr->GetMapMarkerData
MARKER_TYPE BGSLocationEx::GetMapMarkerType()
{
	if (auto marker = worldLocMarker.get()) {
		ExtraMapMarker* extra = marker->extraList.GetByType<ExtraMapMarker>();
		if (extra) {
			return extra->mapData->type.get();
		}
	}
	return MARKER_TYPE::kNone;
}

BGSLocationEx* BGSLocationEx::GetLastChecked()
{
	return lastChecked;
}

void BGSLocationEx::Install(SKSE::Trampoline& trampoline)
{
	trampoline.write_call<5>(Offset::BGSLocation::CanLocBeAlias.address() + OFFSET(0x3F, 0x3F, 0x3F), CheckLocationCleared_Hook);
	trampoline.write_call<5>(Offset::GetAllGoodLocationsFunctor::Run.address() + OFFSET(0x71, 0xAE, 0x71), CheckLocationCleared_Hook);
	trampoline.write_call<5>(Offset::Actor::Resurrect.address() + OFFSET(0x386, 0x347, 0x386), CheckLocationCleared_Hook);
	_CheckLocationCleared = trampoline.write_call<5>(Offset::Actor::KillImpl.address() + OFFSET(0x1142, 0x11E8, 0x1142), CheckLocationCleared_Hook);
}

bool BGSLocationEx::CheckLocationCleared_Hook(BGSLocationEx* location, int time, bool force)
{
	lastChecked = location;

	if (location->IsLoaded()) {
		return _CheckLocationCleared(location, time, force);
	}

	return location->IsCleared();
}
