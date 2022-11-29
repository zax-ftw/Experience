#include "ObjectiveState.h"

#include "Offsets.h"

namespace RE
{
	BSTEventSource<ObjectiveState::Event>* ObjectiveState::GetEventSource()
	{
		using func_t = decltype(&ObjectiveState::GetEventSource);
		REL::Relocation<func_t> func{ RE::Offset::StoryEvents::ObjectiveState };
		return func();
	}
}
