#include "Events/QuestStatus.h"

#include "Settings.h"

using namespace RE;

QuestStatusEventHandler::QuestStatusEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager)
{
	QuestStatus::GetEventSource()->AddEventSink(this);
}

QuestStatusEventHandler::~QuestStatusEventHandler()
{
	QuestStatus::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl QuestStatusEventHandler::ProcessEvent(const QuestStatus::Event* event, QuestStatusEventSource*)
{
	if (event->status != QuestStatus::kCompleted || !event->quest) {
		return BSEventNotifyControl::kContinue;
	}

	TESQuest* quest = event->quest;

	if (IsValidCompletion(quest)) {

		auto questType = quest->GetType();

		logger::info("[QuestCompleted] {0} ({1})", 
			quest->GetFormEditorID(), EnumToString(questType));

		auto reward = GetReward(questType);

		AddExperience(reward, questType != QuestType::kMiscellaneous);	
	}

	return BSEventNotifyControl::kContinue;
}

bool QuestStatusEventHandler::IsValidCompletion(TESQuest *quest)
{
	if (quest->GetType() == QuestType::kMiscellaneous){
		return GetNumObjectivesCompleted(quest) > 0;
	}
	return quest->GetName()[0];
}

uint8_t QuestStatusEventHandler::GetNumObjectivesCompleted(TESQuest *quest)
{
	uint8_t completedCount = 0;
	for (auto objective : quest->objectives) {
		if (objective->state.get() == QUEST_OBJECTIVE_STATE::kCompletedDisplayed) {
			completedCount++;
		}
	}
	return completedCount;
}

int QuestStatusEventHandler::GetReward(QuestType type)
{
	auto settings = Settings::GetSingleton();

	switch (type) {
	case QuestType::kNone:
		return settings->GetValue<int>("iXPQuestNone");
	case QuestType::kMainQuest:
		return settings->GetValue<int>("iXPQuestMain");
	case QuestType::kMagesGuild:
		return settings->GetValue<int>("iXPQuestCollege");
	case QuestType::kThievesGuild:
		return settings->GetValue<int>("iXPQuestThieves");
	case QuestType::kDarkBrotherhood:
		return settings->GetValue<int>("iXPQuestBrotherhood");
	case QuestType::kCompanionsQuest:
		return settings->GetValue<int>("iXPQuestCompanions");
	case QuestType::kMiscellaneous:
		return settings->GetValue<int>("iXPQuestMisc");
	case QuestType::kDaedric:
		return settings->GetValue<int>("iXPQuestDaedric");
	case QuestType::kSideQuest:
		return settings->GetValue<int>("iXPQuestSide");
	case QuestType::kCivilWar:
		return settings->GetValue<int>("iXPQuestCivilWar");
	case QuestType::kDLC01_Vampire:
		return settings->GetValue<int>("iXPQuestDawnguard");
	case QuestType::kDLC02_Dragonborn:
		return settings->GetValue<int>("iXPQuestDragonborn");
	default:
		logger::warn("Unknown quest type: {}", static_cast<int>(type));
		return settings->GetValue<int>("iXPQuestNone");
	}
}
