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

		if (name && strlen(name) != 0) {

			auto edid = quest->GetFormEditorID();
			auto type = quest->GetType();

			auto text = remove_prefix(enum_name(type));
			logger::info("QuestCompleted: {0} ({1})", edid, text);

			auto reward = GetReward(type);
			AddExperience(reward);
		}
	}
	return BSEventNotifyControl::kContinue;
}

int QuestStatusEventHandler::GetReward(QuestType type)
{
	Settings& settings = Settings::GetSingleton();

	switch (type) {
	case QuestType::kMainQuest:
		return settings.GetSettingInt("iXPQuestMain");
	case QuestType::kMagesGuild:
		return settings.GetSettingInt("iXPQuestCollege");
	case QuestType::kThievesGuild:
		return settings.GetSettingInt("iXPQuestThieves");
	case QuestType::kDarkBrotherhood:
		return settings.GetSettingInt("iXPQuestBrotherhood");
	case QuestType::kCompanionsQuest:
		return settings.GetSettingInt("iXPQuestCompanions");
	case QuestType::kMiscellaneous:
		return settings.GetSettingInt("iXPQuestMisc");
	case QuestType::kDaedric:
		return settings.GetSettingInt("iXPQuestDaedric");
	case QuestType::kSideQuest:
		return settings.GetSettingInt("iXPQuestSide");
	case QuestType::kCivilWar:
		return settings.GetSettingInt("iXPQuestCivilWar");
	case QuestType::kDLC01_Vampire:
		return settings.GetSettingInt("iXPQuestDawnguard");
	case QuestType::kDLC02_Dragonborn:
		return settings.GetSettingInt("iXPQuestDragonborn");
	default:
		return settings.GetSettingInt("iXPQuestNone");
	}
}
