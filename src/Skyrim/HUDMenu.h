#pragma once

class HUDMenuEx : public RE::HUDMenu
{
public:

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

	static void ShowNotification(const char* text, const char* status, const char* sound);
	static void ShowLevelMeter(uint16_t level, float startPercent, float endPercent);

private:

	void ShowNotification_Impl(const char* text, const char* status, const char* sound);
	void ShowLevelMeter_Impl(uint16_t level, float startPercent, float endPercent);

	RE::HUDObject* GetElement(ObjectType type);

};

class HUDNotificationsEx : public RE::HUDNotifications
{
public:

	void ShowNotification(const char* text, const char* status, const char* sound);
	void ShowLevelMeter(uint16_t level, float startPercent, float endPercent);
};


