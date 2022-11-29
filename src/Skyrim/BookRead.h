#pragma once

#include "RE/B/BSTEvent.h"

namespace RE
{
	class TESObjectBOOK;

	struct BookRead
	{
	public:
		struct Event
		{
			// members
			TESObjectBOOK* book;       // 00
			bool           skillBook;  // 08
		};

		static BSTEventSource<Event>* GetEventSource();
	};
}
