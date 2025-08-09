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

	case static_cast<QuestType>(12):
		return Settings::Questing::XPQuestType12;
	case static_cast<QuestType>(13):
		return Settings::Questing::XPQuestType13;
	case static_cast<QuestType>(14):
		return Settings::Questing::XPQuestType14;
	case static_cast<QuestType>(15):
		return Settings::Questing::XPQuestType15;
	case static_cast<QuestType>(16):
		return Settings::Questing::XPQuestType16;
	case static_cast<QuestType>(17):
		return Settings::Questing::XPQuestType17;
	case static_cast<QuestType>(18):
		return Settings::Questing::XPQuestType18;
	case static_cast<QuestType>(19):
		return Settings::Questing::XPQuestType19;
	case static_cast<QuestType>(20):
		return Settings::Questing::XPQuestType20;
	case static_cast<QuestType>(21):
		return Settings::Questing::XPQuestType21;
	case static_cast<QuestType>(22):
		return Settings::Questing::XPQuestType22;
	case static_cast<QuestType>(23):
		return Settings::Questing::XPQuestType23;
	case static_cast<QuestType>(24):
		return Settings::Questing::XPQuestType24;
	case static_cast<QuestType>(25):
		return Settings::Questing::XPQuestType25;
	case static_cast<QuestType>(26):
		return Settings::Questing::XPQuestType26;
	case static_cast<QuestType>(27):
		return Settings::Questing::XPQuestType27;
	case static_cast<QuestType>(28):
		return Settings::Questing::XPQuestType28;
	case static_cast<QuestType>(29):
		return Settings::Questing::XPQuestType29;
	case static_cast<QuestType>(30):
		return Settings::Questing::XPQuestType30;
	case static_cast<QuestType>(31):
		return Settings::Questing::XPQuestType31;
	case static_cast<QuestType>(32):
		return Settings::Questing::XPQuestType32;
	case static_cast<QuestType>(33):
		return Settings::Questing::XPQuestType33;
	case static_cast<QuestType>(34):
		return Settings::Questing::XPQuestType34;
	case static_cast<QuestType>(35):
		return Settings::Questing::XPQuestType35;
	case static_cast<QuestType>(36):
		return Settings::Questing::XPQuestType36;
	case static_cast<QuestType>(37):
		return Settings::Questing::XPQuestType37;
	case static_cast<QuestType>(38):
		return Settings::Questing::XPQuestType38;
	case static_cast<QuestType>(39):
		return Settings::Questing::XPQuestType39;
	case static_cast<QuestType>(40):
		return Settings::Questing::XPQuestType40;
	case static_cast<QuestType>(41):
		return Settings::Questing::XPQuestType41;
	case static_cast<QuestType>(42):
		return Settings::Questing::XPQuestType42;
	case static_cast<QuestType>(43):
		return Settings::Questing::XPQuestType43;
	case static_cast<QuestType>(44):
		return Settings::Questing::XPQuestType44;
	case static_cast<QuestType>(45):
		return Settings::Questing::XPQuestType45;
	case static_cast<QuestType>(46):
		return Settings::Questing::XPQuestType46;
	case static_cast<QuestType>(47):
		return Settings::Questing::XPQuestType47;
	case static_cast<QuestType>(48):
		return Settings::Questing::XPQuestType48;
	case static_cast<QuestType>(49):
		return Settings::Questing::XPQuestType49;
	case static_cast<QuestType>(50):
		return Settings::Questing::XPQuestType50;
	case static_cast<QuestType>(51):
		return Settings::Questing::XPQuestType51;
	case static_cast<QuestType>(52):
		return Settings::Questing::XPQuestType52;
	case static_cast<QuestType>(53):
		return Settings::Questing::XPQuestType53;
	case static_cast<QuestType>(54):
		return Settings::Questing::XPQuestType54;
	case static_cast<QuestType>(55):
		return Settings::Questing::XPQuestType55;
	case static_cast<QuestType>(56):
		return Settings::Questing::XPQuestType56;
	case static_cast<QuestType>(57):
		return Settings::Questing::XPQuestType57;
	case static_cast<QuestType>(58):
		return Settings::Questing::XPQuestType58;
	case static_cast<QuestType>(59):
		return Settings::Questing::XPQuestType59;
	case static_cast<QuestType>(60):
		return Settings::Questing::XPQuestType60;
	case static_cast<QuestType>(61):
		return Settings::Questing::XPQuestType61;
	default:
		logger::warn("Unknown quest type: {}", static_cast<int>(type));
		return Settings::Questing::XPQuestSide;
	}
}
