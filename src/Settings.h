#pragma once

#include "Utils/Singleton.h"


class Settings : public Singleton<Settings>
{
	friend class Singleton<Settings>;
public:

	void LoadSettings();
	void SaveSettings();

	template <typename T>
	[[nodiscard]] T GetSetting(std::string_view key) const
	{
		auto it = data.find(key);
		if (it == data.end())
			return T();

		return std::get<T>(it->second);  // std::get_if
	};

	template <typename T>
	void SetSetting(std::string_view key, T value)
	{
		data[key] = value;
	};

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

	std::map<std::string_view, std::variant<int, float, bool, std::string>> data;

	std::string path;
};
