#pragma once

#include <shared_mutex>

#include "Utils/Singleton.h"

template <typename, typename>
constexpr bool is_one_of_variants_types = false;

template <typename... Ts, typename T>
constexpr bool is_one_of_variants_types<std::variant<Ts...>, T> = (std::is_same_v<T, Ts> || ...);

class Settings : public ISingleton<Settings>
{
public:
	using Setting = std::variant<std::monostate, int, float, bool, std::string>;
	using SettingsMap = std::unordered_map<std::string_view, Setting>;

	friend class ISingleton<Settings>;

	void ReadSettings();
	void WriteSettings();

	template <typename T>
	auto GetValue(std::string_view key) const
		-> std::enable_if_t<is_one_of_variants_types<Setting, T>,T>
	{
		auto setting = GetSetting(key);
		if (std::holds_alternative<T>(setting)) {
			return std::get<T>(setting);
		}
		return T();
	}

	template <typename T>
	auto SetValue(std::string_view key, T value)
		-> std::enable_if_t<is_one_of_variants_types<Setting, T>>
	{
		std::unique_lock lock(mtx);
		data.insert_or_assign(key, value);
	};

private:

	Settings() {};
	~Settings() override {};

	Setting GetSetting(std::string_view key) const;

	int ReadIntSetting(CSimpleIniA& ini, const char* section, const char* key, int def);
	float ReadFloatSetting(CSimpleIniA& ini, const char* section, const char* key, float def);
	bool ReadBoolSetting(CSimpleIniA& ini, const char* section, const char* key, bool def);
	const char* ReadStringSetting(CSimpleIniA& ini, const char* section, const char* key, const char* def);

	void WriteIntSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment = nullptr);
	void WriteFloatSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment = nullptr);
	void WriteBoolSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment = nullptr);
	void WriteStringSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment = nullptr);

	static constexpr const char* path = "Data/SKSE/Plugins/Experience.ini";

	mutable std::shared_mutex mtx;
	SettingsMap data;
};
