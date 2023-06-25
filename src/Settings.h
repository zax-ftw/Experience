#pragma once

#include "Utils/Singleton.h"


class Settings : public ISingleton<Settings>
{
public:
	using Setting = std::variant<std::monostate, int, float, bool, std::string>;
	using SettingsMap = std::map<std::string_view, Setting>;

	friend class ISingleton<Settings>;

	void LoadSettings();
	void SaveSettings();

	Setting GetSetting(std::string_view key) const
	{
		auto it = data.find(key);
		if (it == data.end())
			return Setting();

		return it->second;
	};

	void SetSetting(std::string_view key, Setting value)
	{
		data.insert_or_assign(key, value);
	};

	template <typename T>
	[[nodiscard]] T GetSetting(std::string_view key) const 
	{ 
		auto setting = GetSetting(key);
		if (std::holds_alternative<T>(setting)) {
			return std::get<T>(setting);
		}
		return T();
	}

	auto GetSettingInt(std::string_view key) const { return GetSetting<int>(key); }
	auto GetSettingFloat(std::string_view key) const { return GetSetting<float>(key); }
	auto GetSettingBool(std::string_view key) const { return GetSetting<bool>(key); }
	auto GetSettingString(std::string_view key) const { return GetSetting<std::string>(key); }

	void SetSettingInt(std::string_view key, int value) { SetSetting(key, value); };
	void SetSettingFloat(std::string_view key, float value) { SetSetting(key, value); };
	void SetSettingBool(std::string_view key, bool value) { SetSetting(key, value); };
	void SetSettingString(std::string_view key, std::string value) { SetSetting(key, value); };

private:

	Settings();
	~Settings(){};

	SettingsMap data;
	std::string path;
};
