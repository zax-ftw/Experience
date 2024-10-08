#include "Skyrim/HUDMenu.h"

#include "Offsets.h"
#include "Utils/GameSettings.h"

using namespace RE;

HUDMenuEx* HUDMenuEx::GetInstance()
{
	auto strings = InterfaceStrings::GetSingleton();
	auto menu = UI::GetSingleton()->GetMenu(strings->hudMenu);
	
	return menu ? static_cast<HUDMenuEx*>(menu.get()) : nullptr;		
}

void HUDMenuEx::ShowNotification(const char* text, const char* status, const char* sound)
{
	if (auto menu = GetInstance()) {
		menu->ShowNotification_Impl(text, status, sound);
	}
}

void HUDMenuEx::ShowLevelMeter(uint16_t level, float startPercent, float endPercent)
{
	if (auto menu = GetInstance()) {
		menu->ShowLevelMeter_Impl(level, startPercent, endPercent);
	}
}

void HUDMenuEx::ShowNotification_Impl(const char* text, const char* status, const char* sound)
{
	logger::trace("ShowNotification, text: {0}, status: {1}, sound: {2}", text, status, sound);

	HUDNotificationsEx* notifications = static_cast<HUDNotificationsEx*>(
		GetElement<HUDNotifications>());

	if (notifications) {
		notifications->ShowNotification(text, status, sound);
	}
}

void HUDMenuEx::ShowLevelMeter_Impl(uint16_t level, float startPercent, float endPercent)
{
	logger::trace("ShowLevelMeter, level: {0}, {1:.2f} => {2:.2f}", level, startPercent, endPercent);

	HUDNotificationsEx* notifications = static_cast<HUDNotificationsEx*>(
		GetElement<HUDNotifications>());

	if (notifications) {
		notifications->ShowLevelMeter(level, startPercent, endPercent);
	}
}

void HUDNotificationsEx::ShowNotification(const char* text, const char* status, const char* sound)
{
	Notification notification { .text = text, .status = status, .sound = sound, .type = 14 };

	int waitTime = Utils::GetGameSettingInt("iObjectivesWaitTime:Interface");
	notification.time = waitTime + GetDurationOfApplicationRunTime();

	queue.push_back(notification);
}

void HUDNotificationsEx::ShowLevelMeter(uint16_t level, float startPercent, float endPercent)
{
	SKSE::GetTaskInterface()->AddUITask([=] {

		GFxValue args[3];
		args[0].SetNumber(level);
		args[1].SetNumber(startPercent);
		args[2].SetNumber(endPercent);

		time = GetDurationOfApplicationRunTime();

		root.Invoke("ShowLevelUpMeter", nullptr, args, 3);
	});
}


