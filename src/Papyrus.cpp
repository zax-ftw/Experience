#include "Papyrus.h"

#include "Experience.h"
#include "Settings/Settings.h"
#include "Skyrim/HUDMenu.h"

#include "Hooks/PlayerSkills.h"

using namespace RE;

namespace Papyrus
{
	BSFixedString GetVersion(StaticFunctionTag*)
	{
		return Plugin::VERSION.string(".");
	}

	void AddExperience(StaticFunctionTag*, int points, bool meter)
	{
		ExperienceManager::Experience exp {.points = points, .widget = meter};

		ExperienceManager::GetSingleton()->AddExperience(exp);
	}

	void ShowNotification(StaticFunctionTag*, BSFixedString text, BSFixedString status, BSFixedString sound)
	{
		HUDMenuEx::ShowNotification(text.c_str(), status.c_str(), sound.c_str());
	}

	float GetSkillCap(StaticFunctionTag*, ActorValue skill)
	{		
		return PlayerSkillsEx::GetSkillCap(skill);
	}

	int32_t GetSettingInt(StaticFunctionTag*, BSFixedString setting)
	{
		return Settings::GetSingleton()->GetValue<int>(setting);
	}

	float GetSettingFloat(StaticFunctionTag*, BSFixedString setting)
	{
		return Settings::GetSingleton()->GetValue<float>(setting);
	}

	bool GetSettingBool(StaticFunctionTag*, BSFixedString setting)
	{
		return Settings::GetSingleton()->GetValue<bool>(setting);
	}

	BSFixedString GetSettingString(StaticFunctionTag*, BSFixedString setting)
	{
		return Settings::GetSingleton()->GetValue<std::string>(setting);
	}

	void SetSettingInt(StaticFunctionTag*, BSFixedString setting, int32_t value)
	{
		return Settings::GetSingleton()->SetValue<int>(setting, value);
	}

	void SetSettingFloat(StaticFunctionTag*, BSFixedString setting, float value)
	{
		return Settings::GetSingleton()->SetValue<float>(setting, value);
	}

	void SetSettingBool(StaticFunctionTag*, BSFixedString setting, bool value)
	{
		return Settings::GetSingleton()->SetValue<bool>(setting, value);
	}

	void SetSettingString(StaticFunctionTag*, BSFixedString setting, BSFixedString value)
	{
		return Settings::GetSingleton()->SetValue<std::string>(setting, value.c_str());
	}

	void SaveSettings(StaticFunctionTag*)
	{
		Settings::GetSingleton()->WriteSettings();
	}

	void LoadSettings(StaticFunctionTag*)
	{
		Settings::GetSingleton()->ReadSettings();
	}

	void DumpSettings(StaticFunctionTag*)
	{
		Settings::GetSingleton()->ReadSettings();
	}

	bool RegisterFuncs(BSScript::IVirtualMachine* vm)
	{
		vm->RegisterFunction("GetVersion", "Experience", GetVersion);
		vm->RegisterFunction("AddExperience", "Experience", AddExperience);
		vm->RegisterFunction("ShowNotification", "Experience", ShowNotification);
		vm->RegisterFunction("GetSkillCap", "Experience", GetSkillCap);

		vm->RegisterFunction("GetSettingInt", "Experience", GetSettingInt);		
		vm->RegisterFunction("GetSettingFloat", "Experience", GetSettingFloat);
		vm->RegisterFunction("GetSettingBool", "Experience", GetSettingBool);
		vm->RegisterFunction("GetSettingString", "Experience", GetSettingString);

		vm->RegisterFunction("SetSettingInt", "Experience", SetSettingInt);
		vm->RegisterFunction("SetSettingFloat", "Experience", SetSettingFloat);
		vm->RegisterFunction("SetSettingBool", "Experience", SetSettingBool);
		vm->RegisterFunction("SetSettingString", "Experience", SetSettingString);

		vm->RegisterFunction("SaveSettings", "Experience", SaveSettings);
		vm->RegisterFunction("LoadSettings", "Experience", LoadSettings);
		vm->RegisterFunction("DumpSettings", "Experience", DumpSettings);

		logger::info("Papyrus functions registered");

		return true;
	}
}
