#include "Hooks/BGSLocation.h"

#include "Offsets.h"

using namespace RE;

BGSLocationEx* BGSLocationEx::lastChecked = nullptr;

bool BGSLocationEx::ClearedCheck(int time, bool force)
{
	using func_t = decltype(&BGSLocationEx::ClearedCheck);
	REL::Relocation<func_t> func{ Offset::BGSLocation::ClearedCheck };
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

void BGSLocationEx::Install(SKSE::Trampoline& trampoline)
{
	_ClearedCheck = trampoline.write_call<5>(Offset::BGSLocation::TryToClear.address() + OFFSET(0x3F, 0x3F, 0x3F), ClearedCheck_Hook);
	trampoline.write_call<5>(Offset::Unknown::LocationMarkerRelated.address() + OFFSET(0x71, 0xAE, 0x71), ClearedCheck_Hook);
	trampoline.write_call<5>(Offset::Actor::Resurrect.address() + OFFSET(0x386, 0x347, 0x386), ClearedCheck_Hook);
	trampoline.write_call<5>(Offset::Actor::Kill.address() + OFFSET(0x1142, 0x11E8, 0x1142), ClearedCheck_Hook);
}

bool BGSLocationEx::ClearedCheck_Hook(BGSLocationEx* location, int time, bool force)
{
	lastChecked = location;

	return _ClearedCheck(location, time, force);
}
