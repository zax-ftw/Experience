#pragma once

#include "SimpleIni.h"
#include <shared_mutex>

class ISettingField
{
public:
	struct Flags
	{
		bool loaded : 1;
		bool modified : 1;
		mutable bool used : 1;
	};

	virtual ~ISettingField() = default;
	virtual void Load(CSimpleIniA& ini) = 0;
	virtual void Save(CSimpleIniA& ini, bool force) const = 0;
	virtual const std::string& GetKey() const = 0;
	virtual const std::string& GetSection() const = 0;
	virtual std::string ToString() const = 0;

    std::string GetFlagsString() const
	{
		std::string out;
		if (flags.loaded)
			out += 'L';
		if (flags.modified)
			out += 'M';
		if (flags.used)
			out += 'U';
		return out;
	}

protected:

	Flags flags {};
};

class Settings;

template <typename T>
constexpr bool is_supported_setting_v =
	std::is_same_v<T, float> ||
	std::is_same_v<T, bool> ||
	std::is_same_v<T, int> ||
	std::is_same_v<T, std::string>;

template <typename T, typename = std::enable_if_t<is_supported_setting_v<T>>>
class SettingField : public ISettingField
{
public:

	SettingField(std::string_view section, std::string_view key, T value) :
		SettingField(section, key, value, "") {}

	SettingField(std::string_view section, std::string_view key, T value, std::string_view comment) :
		key(key), value(value), comment(comment), section(section)
	{
		Settings::GetSingleton()->Register(this);
	}

	const std::string& GetKey() const override { return key; }
	const std::string& GetSection() const override { return section; }
	std::string ToString() const override;

	operator T() const { return value; }

	SettingField& operator=(const T& val)
	{
		Set(val);
		return *this;
	}

	T Get() const 
	{
		std::shared_lock lock(mtx);
		flags.used = true;
		return value; 
	}

	void Set(T val) 
	{
		std::unique_lock lock(mtx);
		if (value != val) {
			value = val;
			flags.modified = true;
		}
	}

	void Load(CSimpleIniA& ini) override;
	void Save(CSimpleIniA& ini, bool force) const override;

private:

	std::string key;
	std::string section;
	std::string comment;
	T value;
	mutable std::shared_mutex mtx;
};
