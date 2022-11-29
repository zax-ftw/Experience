#include "QuestStatus.h"

#include "Offsets.h"

namespace RE
{
	BSTEventSource<QuestStatus::Event>* QuestStatus::GetEventSource()
	{
		using func_t = decltype(&QuestStatus::GetEventSource);
		REL::Relocation<func_t> func{ RE::Offset::StoryEvents::QuestStatus };
		return func();
	}
}
