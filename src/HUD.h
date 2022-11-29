#pragma once

namespace HUD
{
	using Notification = RE::HUDNotifications::Notification;
	//using ObjectType = RE::HUDObject::Type;
	//using MessageType = RE::HUDData::Type;

	enum ObjectType
	{
		kHealthMeter,
		kMagickaMeter,
		kStaminaMeter,
		kShoutMeter,
		kEnemyHealth,
		kCompass,
		kHUDNotifications,
		kStealthMeter,
		kHUDChargeMeter,
		kFloatingQuestMarker,
		kTotal
	};

	enum MessageType
	{
		kShowMessage = 1,
		kSetCrosshairTarget = 3,
		kSetLoadDoorInfo = 4,
		kShowSubtitle = 5,
		kHideSubtitle = 6,
		kShowArrowCount = 7,
		kHideArrowCount = 8,
		kSetSubtitlesEnabled = 9,
		kQuestStarted = 14,
		kQuestCompleted = 15,
		kQuestUpdated = 16,
		kNewObjective = 17,
		kObjectiveCompleted = 18,
		kObjectiveFailed = 19,
		kSkillIncreased = 20,
		kWorldOfPowerLearned = 21,
		kDragonSoulAbsorbed = 22,
		kShowElements = 23,
		kLocationDiscoverd = 26,
		kFavor = 27,
		kValidateCrosshair = 28,
		kSetLocationName = 29,
		kShowTutrialHintText = 30,
		kSetCrosshairEnabled = 31,
		kUnk32 = 32,
		kRefreshActivateButtonArt = 33,
		kUnk34 = 34
	};

	void ShowNotification(const char* text, const char* status, const char* sound, MessageType type);
	void ShowLevelMeter(uint16_t level, float startPercent, float endPercent);

	RE::HUDObject* GetElement(RE::HUDMenu* hud, ObjectType type);

	namespace Impl
	{
		void ShowNotification(RE::HUDNotifications* object, const char* text, const char* status, const char* sound, MessageType type);
		void ShowLevelMeter(RE::HUDNotifications* object, uint16_t level, float startPercent, float endPercent);
	};

};

