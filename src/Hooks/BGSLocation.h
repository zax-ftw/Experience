#pragma once

class BGSLocationEx : public RE::BGSLocation
{
public:
	static void Install(REL::Trampoline& trampoline);
};
