#include "Hooks/BGSLocation.h"

#include "Offsets.h"

using namespace RE;

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

template <int N>
bool BGSLocationEx::CheckCleared_Hook(BGSLocationEx* location, int time, bool force)
{
	lastChecked = location;

	if (location->IsLoaded()) {
		return _CheckCleared[N](location, time, force);
	}

	return location->IsCleared();
}

void BGSLocationEx::Install(SKSE::Trampoline& trampoline)
{
	_CheckCleared[0] = trampoline.write_call<5>(Offset::BGSLocation::CanLocBeAlias.address() + OFFSET(0x3F, 0x3F, 0x3F), CheckCleared_Hook<0>);
	_CheckCleared[1] = trampoline.write_call<5>(Offset::GetAllGoodLocationsFunctor::Run.address() + OFFSET(0x71, 0xAE, 0x71), CheckCleared_Hook<1>);
	_CheckCleared[2] = trampoline.write_call<5>(Offset::Actor::Resurrect.address() + OFFSET(0x386, 0x347, 0x386), CheckCleared_Hook<2>);
	_CheckCleared[3] = trampoline.write_call<5>(Offset::Actor::KillImpl.address() + OFFSET(0x1142, 0x11E8, 0x1142), CheckCleared_Hook<3>);
}
