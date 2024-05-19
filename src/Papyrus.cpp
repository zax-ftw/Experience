#include "Papyrus.h"

#include "Experience.h"
#include "Settings.h"
#include "HUD.h"

#include "Hooks/PlayerSkills.h"

using namespace RE;

namespace Papyrus
{
	BSFixedString GetVersion(StaticFunctionTag*)
	{
		auto plugin = SKSE::PluginDeclaration::GetSingleton();
		auto version = plugin->GetVersion();

		return version.string("."sv);
	}

	void AddExperience(StaticFunctionTag*, float points, bool meter)
	{
		ExperienceManager::Experience experience;

		experience.points = points;
		experience.widget = meter;
		experience.source = nullptr;

		ExperienceManager::GetSingleton()->AddExperience(experience);
	}

	void ShowNotification(StaticFunctionTag*, BSFixedString text, BSFixedString status, BSFixedString sound)
	{
		using Type = HUD::MessageType;

		HUD::ShowNotification(text.c_str(), status.c_str(), sound.c_str(), Type::kQuestStarted);
	}

	float GetSkillCap(StaticFunctionTag*, ActorValue skill)
	{		
		return PlayerSkillsEx::GetSkillCap1(skill);
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
		return Settings::GetSingleton()->WriteSettings();
	}

	void LoadSettings(StaticFunctionTag*)
	{
		return Settings::GetSingleton()->ReadSettings();
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

		logger::info("Papyrus functions registered");

		return true;
	}
}
