#include "Events/QuestStatus.h"

#include "Settings/Settings.h"

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

		logger::info("[QuestCompleted] {} (type: {})", 
			quest->GetFormEditorID(), static_cast<int>(questType));

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
	switch (type) {
	case QuestType::kNone:
		return Settings::Questing::XPQuestNone;
	case QuestType::kMainQuest:
		return Settings::Questing::XPQuestMain;
	case QuestType::kMagesGuild:
		return Settings::Questing::XPQuestCollege;
	case QuestType::kThievesGuild:
		return Settings::Questing::XPQuestThieves;
	case QuestType::kDarkBrotherhood:
		return Settings::Questing::XPQuestBrotherhood;
	case QuestType::kCompanionsQuest:
		return Settings::Questing::XPQuestCompanions;
	case QuestType::kMiscellaneous:
		return Settings::Questing::XPQuestMisc;
	case QuestType::kDaedric:
		return Settings::Questing::XPQuestDaedric;
	case QuestType::kSideQuest:
		return Settings::Questing::XPQuestSide;
	case QuestType::kCivilWar:
		return Settings::Questing::XPQuestCivilWar;
	case QuestType::kDLC01_Vampire:
		return Settings::Questing::XPQuestDawnguard;
	case QuestType::kDLC02_Dragonborn:
		return Settings::Questing::XPQuestDragonborn;
	default:
		logger::warn("Unknown quest type: {}", static_cast<int>(type));
		return Settings::Questing::XPQuestSide;
	}
}
