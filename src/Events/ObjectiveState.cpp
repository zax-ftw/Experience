#include "Events/ObjectiveState.h"

#include "Offsets.h"
#include "Settings.h"

using namespace RE;

void FillQuestInstanceData(BSString* text, const TESQuest* quest, uint32_t instanceID)
{
	using func_t = decltype(&FillQuestInstanceData);
	REL::Relocation<func_t> func{ Offset::BGSQuestObjective::FillQuestInstanceData };
	return func(text, quest, instanceID);
}

ObjectiveStateEventHandler::ObjectiveStateEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kInactive)
{
	ObjectiveState::GetEventSource()->AddEventSink(this);
}

ObjectiveStateEventHandler::~ObjectiveStateEventHandler(void)
{
	ObjectiveState::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl ObjectiveStateEventHandler::ProcessEvent(const ObjectiveState::Event* event, ObjectiveStateEventSource*)
{
	BGSQuestObjective* objective = event->objective;
	TESQuest* quest = objective->ownerQuest;

	BSString displayText(objective->displayText);
	FillQuestInstanceData(&displayText, quest, quest->currentInstanceID);
	
	logger::info("[ObjectiveState] {0} ({1})", 
		displayText.c_str(), magic_enum::enum_name(event->newState));

	if (quest->GetType() != QuestType::kNone) {

		int reward = Settings::GetSingleton()->GetValue<int>("iXPObjectives");

		if (IsCompleted(event->newState) && !IsCompleted(event->oldState)) {
			AddExperience(reward);
		} else if (IsCompleted(event->oldState) && !IsCompleted(event->newState)) {
			AddExperience(-reward);
		}
	}

	return BSEventNotifyControl::kContinue;
}

bool ObjectiveStateEventHandler::IsCompleted(QUEST_OBJECTIVE_STATE state)
{
	using enum QUEST_OBJECTIVE_STATE;
	return state == kCompleted || state == kCompletedDisplayed;
}
