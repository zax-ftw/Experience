#include "Events/ObjectiveState.h"

#include "Offsets.h"
#include "Settings.h"

using namespace RE;

void ParseString(BSString* format, const TESQuest* quest, uint32_t instanceID)
{
	using func_t = decltype(&ParseString);
	REL::Relocation<func_t> func{ Offset::BGSQuestInstanceText::ParseString };
	return func(format, quest, instanceID);
}

ObjectiveStateEventHandler::ObjectiveStateEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager)
{
	RE::ObjectiveState::GetEventSource()->AddEventSink(this);
}

ObjectiveStateEventHandler::~ObjectiveStateEventHandler()
{
	RE::ObjectiveState::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl ObjectiveStateEventHandler::ProcessEvent(const RE::ObjectiveState::Event* event, ObjectiveStateEventSource*)
{
	logger::info("[ObjectiveState] {0} ({1})",
		GetFormattedDisplayText(event->objective),
		EnumToString(event->newState));

	auto reward = GetReward(event->oldState, event->newState);
	AddExperience(reward, false);

	return BSEventNotifyControl::kContinue;
}

std::string ObjectiveStateEventHandler::GetFormattedDisplayText(BGSQuestObjective* objective)
{
	const TESQuest* quest = objective->ownerQuest;

	BSString displayText(objective->displayText);
	ParseString(&displayText, quest, quest->currentInstanceID);

	return displayText.c_str();
}

int ObjectiveStateEventHandler::GetReward(ObjectiveState oldState, ObjectiveState newState)
{
	const auto base = Settings::GetSingleton()->GetValue<int>("iXPQuestObjectives");

    auto getReward = [base](ObjectiveState state) -> int {
		return (state == ObjectiveState::kCompletedDisplayed) ? base : 0;
	};

	return getReward(newState) - getReward(oldState);
}
