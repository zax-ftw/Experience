#include "LocationCleared.h"

#include "Offsets.h"

namespace RE
{
	BSTEventSource<LocationCleared::Event>* LocationCleared::GetEventSource()
	{
		using func_t = decltype(&LocationCleared::GetEventSource);
		REL::Relocation<func_t> func{ RE::Offset::StoryEvents::LocationCleared };
		return func();
	}
}
