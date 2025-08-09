#include "Settings.h"

void Settings::ReadSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	if (ini.LoadFile(path) == SI_FILE) {
		logger::warn("Failed to load file: {}", path);
	}

	std::shared_lock lock(mtx);
	for (auto* field : fields)
		field->Load(ini);
}

void Settings::WriteSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	std::shared_lock lock(mtx);
	for (auto field : fields)
		field->Save(ini);

	if (ini.SaveFile(path) == SI_FILE) {
		logger::warn("Failed to save file: {}", path);
	}
}

void Settings::DumpSettings() const
{
	logger::info("---- Dumping Settings ----");

	// Group fields by section name
	std::unordered_map<std::string_view, std::vector<const ISettingField*>> grouped;

	std::shared_lock lock(mtx);

	for (const auto field : fields) {
		grouped[field->GetSection()].push_back(field);
	}

	for (const auto& [sectionName, fields] : grouped) {
		logger::info("[{}]", sectionName);
		for (const auto* field : fields) {
			const auto flags = field->GetFlagsString();
			if (!flags.empty()) {
				logger::info("  {} = {} [{}]", field->GetKey(), field->ToString(), flags);
			} else {
				logger::info("  {} = {}", field->GetKey(), field->ToString());
			}
		}
	}
	logger::info("---- End Settings Dump ----");
}

void Settings::Register(ISettingField* field)
{
	std::unique_lock lock(mtx);
	fields.push_back(field);
}

template <typename T>
T Settings::GetValue(std::string_view key) const
{
	std::shared_lock lock(mtx);
	for (const auto f : fields) {
		if (f->GetKey() == key) {
			if (const auto* typed = dynamic_cast<const SettingField<T>*>(f))
				return typed->Get();
		}
	}
	return T();
}

template <typename T>
void Settings::SetValue(std::string_view key, T value)
{
	std::shared_lock lock(mtx);
	for (auto f : fields) {
		if (f->GetKey() == key) {
			if (auto* typed = dynamic_cast<SettingField<T>*>(f)) {
				typed->Set(value);
				return;
			}
		}
	}
}

template float Settings::GetValue<float>(std::string_view) const;
template bool  Settings::GetValue<bool>(std::string_view) const;
template int   Settings::GetValue<int>(std::string_view) const;
template auto  Settings::GetValue<std::string>(std::string_view) const -> std::string;

template void Settings::SetValue<float>(std::string_view, float);
template void Settings::SetValue<bool>(std::string_view, bool);
template void Settings::SetValue<int>(std::string_view, int);
template void Settings::SetValue<std::string>(std::string_view, std::string);
