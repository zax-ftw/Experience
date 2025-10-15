#include "Hooks/BGSLocation.h"

#include "Offsets.h"

using namespace RE;

void BGSLocationEx::Install(SKSE::Trampoline& trampoline)
{
#ifdef SKYRIM_SUPPORT_AE
	/*
		B9 0D 00 00 00 
		E8 28 AE EB FF 
		48 85 C0 
		74 26 
		48 8B 0D 7C A1 D0 02 
		48 8D 54 24 20 
		48 83 C0 20 
		C7 44 24 28 00 00 00 00 
		48 89 44 24 20 
		45 33 C0 
		48 8B 01 
		FF 50 08
	*/
	REL::safe_fill(Offset::BGSLocation::CheckLocationCleared.address() + 0x16E, REL::NOP, 53);
#else
	/*
		E8 6D 3D EB FF
		44 38 B0 8D 0B 00 00
		74 2F
		48 8B 80 88 00 00 00
		48 85 C0
		74 23
		48 8B 0D 21 28 C8 02
		48 8D 54 24 20
		48 83 C0 20
		44 89 74 24 28
		48 89 44 24 20
		45 33 C0
		48 8B 01
		FF 50 08
	*/
	REL::safe_fill(Offset::BGSLocation::CheckLocationCleared.address() + 0x11E, REL::NOP, 61);	
#endif

#ifdef SKYRIM_SUPPORT_AE
	uint8_t kInitEventStruct[] = { 0x48, 0x89, 0xB4, 0x24, 0x80, 0x00, 0x00, 0x00 }; // mov qword ptr [rsp+0x80], rsi
#else
	uint8_t kInitEventStruct[] = { 0x48, 0x89, 0x9C, 0x24, 0x90, 0x00, 0x00, 0x00 }; // mov qword ptr [rsp+0x90], rbx
#endif

	REL::safe_write<uint8_t>(Offset::BGSLocation::CheckLocationCleared.address() + OFFSET(0x11E, 0x16E, 0x11E), kInitEventStruct);
}
