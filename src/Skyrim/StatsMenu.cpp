#include "Skyrim/StatsMenu.h"

#include "Offsets.h"

void StatsMenuEx::UpdateSkillList()
{
	using func_t = decltype(&StatsMenuEx::UpdateSkillList);
	REL::Relocation<func_t> func{ RE::Offset::StatsMenu::UpdateSkillList };
	return func(this);
}
