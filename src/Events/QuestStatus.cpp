#include "Events/QuestStatus.h"

#include "Settings.h"

using namespace RE;

QuestStatusEventHandler::QuestStatusEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kActive)
{
	QuestStatus::GetEventSource()->AddEventSink(this);
}

QuestStatusEventHandler::~QuestStatusEventHandler(void)
{
	QuestStatus::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl QuestStatusEventHandler::ProcessEvent(const QuestStatus::Event* event, QuestStatusEventSource*)
{
	if (event->status == QuestStatus::kCompleted) {

		TESQuest* quest = event->quest;
		auto name = quest->GetName();

		if (name && name[0]) {

			auto edid = quest->GetFormEditorID();
			auto type = quest->GetType();

			logger::info("[QuestCompleted] {0} ({1})", 
				edid, magic_enum::enum_name(type));

			auto reward = GetReward(type);
			AddExperience(reward);
		}
	}
	return BSEventNotifyControl::kContinue;
}

int QuestStatusEventHandler::GetReward(QuestType type)
{
	auto settings = Settings::GetSingleton();

	switch (type) {
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
		return settings->GetValue<int>("iXPQuestNone");
	}
}
