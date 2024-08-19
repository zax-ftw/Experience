#pragma once

class HUDMenuEx : public RE::HUDMenu
{
public:

	static HUDMenuEx* GetInstance();

	static void ShowNotification(const char* text, const char* status, const char* sound);
	static void ShowLevelMeter(uint16_t level, float startPercent, float endPercent);

	template <typename T,
		typename = std::enable_if_t<std::is_base_of_v<RE::HUDObject, T>>>
	T* GetElement() const
	{
		for (auto object : objects) {
			if (auto result = skyrim_cast<T*>(object)) {
				return result;
			}
		}
		return nullptr;
	}

private:

	void ShowNotification_Impl(const char* text, const char* status, const char* sound);
	void ShowLevelMeter_Impl(uint16_t level, float startPercent, float endPercent);
};

class HUDNotificationsEx : public RE::HUDNotifications
{
public:

	void ShowNotification(const char* text, const char* status, const char* sound);
	void ShowLevelMeter(uint16_t level, float startPercent, float endPercent);
};


