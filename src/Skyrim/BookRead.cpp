#include "Skyrim/BookRead.h"

#include "Offsets.h"

namespace RE
{
	BSTEventSource<BookRead::Event>* BookRead::GetEventSource()
	{
		using func_t = decltype(&BookRead::GetEventSource);
		REL::Relocation<func_t> func{ RE::Offset::StoryEvents::BookRead };
		return func();
	}
}
