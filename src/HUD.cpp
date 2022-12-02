#include "HUD.h"

#include "Offsets.h"

using namespace RE;

static uint32_t* realTime = (uint32_t*)Offset::RealTime.address();

void HUD::ShowNotification(const char* text, const char* status, const char* sound, MessageType type)
{
	logger::trace("ShowNotification, text: {0}, status: {1}, sound: {2}", text, status, sound);

	auto hud = UI::GetSingleton()->GetMenu<HUDMenu>();

	HUDNotifications* notifications = skyrim_cast<HUDNotifications*>(
		GetElement(hud.get(), ObjectType::kHUDNotifications));

	if (notifications) {
		Impl::ShowNotification(notifications, text, status, sound, type);
	}
}

void HUD::ShowLevelMeter(uint16_t level, float startPercent, float endPercent)
{
	logger::trace("ShowLevelMeter, level: {0}, {1:.2f} => {2:.2f}", level, startPercent, endPercent);
		
	auto hud = UI::GetSingleton()->GetMenu<HUDMenu>();

	HUDNotifications* notifications = skyrim_cast<HUDNotifications*>(
		GetElement(hud.get(), ObjectType::kHUDNotifications));

	if (notifications) {
		Impl::ShowLevelMeter(notifications, level, startPercent, endPercent);
	}

}

HUDObject* HUD::GetElement(HUDMenu* hud, ObjectType type) 
{ 
	return hud->GetRuntimeData().objects[type];
}

void HUD::Impl::ShowNotification(HUDNotifications* object, const char* text, const char* status, const char* sound, MessageType type)
{
	Notification notification;

	notification.text = text;
	notification.status = status;
	notification.sound = sound;

	auto settings = GameSettingCollection::GetSingleton();

	Setting* setting = settings->GetSetting("iObjectivesWaitTime:Interface");
	int waitTime = setting ? setting->GetSInt() : 250;

	notification.time = *realTime + waitTime;
	notification.type = type;

	object->queue.push_back(notification);
}

void HUD::Impl::ShowLevelMeter(HUDNotifications* object, uint16_t level, float startPercent, float endPercent)
{
	auto task = SKSE::GetTaskInterface();

	task->AddUITask([=] {

		GFxValue args[3];
		args[0].SetNumber(level);
		args[1].SetNumber(startPercent);
		args[2].SetNumber(endPercent);

		object->time = *realTime;
		object->root.Invoke("ShowLevelUpMeter", nullptr, args, 3);
	});
}
