#include "SettingField.h"

template <>
std::string SettingField<float>::ToString() const
{
	std::shared_lock lock(mtx);
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << value;
	return oss.str();
}

template <>
std::string SettingField<bool>::ToString() const
{
	std::shared_lock lock(mtx);
	return value ? "true" : "false";
}

template <>
std::string SettingField<int>::ToString() const
{
	std::shared_lock lock(mtx);
	return std::to_string(value);
}

template <>
std::string SettingField<std::string>::ToString() const
{
	std::shared_lock lock(mtx);
	return value;
}

template <>
void SettingField<float>::Load(CSimpleIniA& ini)
{
	std::unique_lock lock(mtx);
	if (ini.KeyExists(section.c_str(), key.c_str())) {
		flags.loaded = true;
	}
	value = static_cast<float>(ini.GetDoubleValue(section.c_str(), key.c_str(), value));
}
template <>
void SettingField<float>::Save(CSimpleIniA& ini) const
{
	std::shared_lock lock(mtx);
	if (flags.loaded || flags.modified) {
		ini.SetDoubleValue(section.c_str(), key.c_str(), value, comment.empty() ? nullptr : comment.c_str());
	}
}
template <>
void SettingField<bool>::Load(CSimpleIniA& ini)
{
	std::unique_lock lock(mtx);
	if (ini.KeyExists(section.c_str(), key.c_str())) {
		flags.loaded = true;
	}
	value = ini.GetBoolValue(section.c_str(), key.c_str(), value);
}
template <>
void SettingField<bool>::Save(CSimpleIniA& ini) const
{
	std::shared_lock lock(mtx);
	if (flags.loaded || flags.modified) {
		ini.SetBoolValue(section.c_str(), key.c_str(), value, comment.empty() ? nullptr : comment.c_str());
	}
}
template <>
void SettingField<int>::Load(CSimpleIniA& ini)
{
	std::unique_lock lock(mtx);
	if (ini.KeyExists(section.c_str(), key.c_str())) {
		flags.loaded = true;
	}
	value = ini.GetLongValue(section.c_str(), key.c_str(), value);
}
template <>
void SettingField<int>::Save(CSimpleIniA& ini) const
{
	std::shared_lock lock(mtx);
	if (flags.loaded || flags.modified) {
		ini.SetLongValue(section.c_str(), key.c_str(), value, comment.empty() ? nullptr : comment.c_str());
	}
}
template <>
void SettingField<std::string>::Load(CSimpleIniA& ini)
{
	std::unique_lock lock(mtx);
	if (ini.KeyExists(section.c_str(), key.c_str())) {
		flags.loaded = true;
	}
	value = ini.GetValue(section.c_str(), key.c_str(), value.c_str());
}

template <>
void SettingField<std::string>::Save(CSimpleIniA& ini) const
{
	std::shared_lock lock(mtx);
	if (flags.loaded || flags.modified) {
		ini.SetValue(section.c_str(), key.c_str(), value.c_str(), comment.empty() ? nullptr : comment.c_str());
	}
}
