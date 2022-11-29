#include "Events/BooksRead.h"

#include "Settings.h"

#include <math.h> // round, sqrt
#include <bitset>

using namespace RE;

BookReadEventHandler::BookReadEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kInactive)
{
	BookRead::GetEventSource()->AddEventSink(this);
}

BookReadEventHandler::~BookReadEventHandler(void)
{
	BookRead::GetEventSource()->RemoveEventSink(this);
}

// kType_Skill flag doesn't seem to be used, using event->skillBook instead
BSEventNotifyControl BookReadEventHandler::ProcessEvent(const BookRead::Event* event, BookReadEventSource*)
{
	TESObjectBOOK* book = event->book;
	if (!event->skillBook && !book->TeachesSpell())
	{
		logger::info("BookRead: {0} ({1})", book->GetName(), book->value);

		auto reward = GetReward(book->value);
		AddExperience(reward);
		
	}
	return BSEventNotifyControl::kContinue;
}

int BookReadEventHandler::GetReward(float value)
{
	float mult = Settings::GetSingleton().GetSettingFloat("fXPReadingMult");
	float sqrt = std::sqrtf(value * mult);

	return std::round(sqrt);
}
