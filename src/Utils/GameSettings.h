#pragma once

namespace Utils
{
	using SettingType = RE::Setting::Type;

	inline RE::Setting* GetGameSetting(const char* a_setting)
	{
		auto settings = RE::GameSettingCollection::GetSingleton();
		return settings->GetSetting(a_setting);
	}

	inline float GetGameSettingFloat(const char* a_setting, float a_default = 0.0f)
	{
		RE::Setting* setting = GetGameSetting(a_setting);
		if (setting && setting->GetType() == SettingType::kFloat) {
			return setting->GetFloat();
		}
		return a_default;
	}

	inline int GetGameSettingInt(const char* a_setting, int a_default = 0) 
	{
		RE::Setting* setting = GetGameSetting(a_setting);
		if (setting && setting->GetType() == SettingType::kSignedInteger) {
			return setting->GetSInt();
		}
		return a_default;
	}

	inline bool GetGameSettingBool(const char* a_setting, bool a_default = false)
	{
		RE::Setting* setting = GetGameSetting(a_setting);
		if (setting && setting->GetType() == SettingType::kBool) {
			return setting->GetBool();
		}
		return a_default;
	}

	inline const char* GetGameSettingString(const char* a_setting, const char* a_default = "")
	{
		RE::Setting* setting = GetGameSetting(a_setting);
		if (setting && setting->GetType() == SettingType::kString) {
			return setting->GetString();
		}
		return a_default;
	}
}
