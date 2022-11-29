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
	trampoline.write_call<5>(Offset::BGSLocation::TryToClear.address() + 0x3F, &BGSLocationEx::ClearedCheck_Hook);
	trampoline.write_call<5>(Offset::Unknown::LocationMarkerRelated.address() + OFFSET(0x71, 0xAE), &BGSLocationEx::ClearedCheck_Hook);
	trampoline.write_call<5>(Offset::Actor::Resurrect.address() + OFFSET(0x386, 0x347), &BGSLocationEx::ClearedCheck_Hook);
	trampoline.write_call<5>(Offset::Actor::Kill.address() + OFFSET(0x1142, 0x11E8), &BGSLocationEx::ClearedCheck_Hook);
}

bool BGSLocationEx::ClearedCheck_Hook(int time, bool force)
{
	lastChecked = this;

	if (this->loadedCount > 0) {
		return ClearedCheck(time, force);
	}
	return false;
}
