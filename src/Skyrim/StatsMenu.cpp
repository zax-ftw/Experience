#include "Skyrim/StatsMenu.h"

#include "Offsets.h"

void StatsMenuEx::InitSkills()
{
	using func_t = decltype(&StatsMenuEx::InitSkills);
	REL::Relocation<func_t> func{ RE::Offset::StatsMenu::InitSkills };
	return func(this);
}

void StatsMenuEx::UpdateDescriptionCard()
{
	using func_t = decltype(&StatsMenuEx::UpdateDescriptionCard);
	REL::Relocation<func_t> func { RE::Offset::StatsMenu::UpdateDescriptionCard };
	return func(this);
}
