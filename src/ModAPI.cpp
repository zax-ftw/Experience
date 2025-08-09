#include "ModAPI.h"

#include "Experience.h"
#include "Settings/Settings.h"
#include "Skyrim/HUDMenu.h"

#include "Hooks/PlayerSkills.h"


namespace Experience
{
	void ExperienceInterface::AddExperience(float points, bool meter)
	{
		ExperienceManager::Experience experience;

		experience.points = points;
		experience.widget = meter;
		experience.source = nullptr;

		ExperienceManager::GetSingleton()->AddExperience(experience);
	}

	float ExperienceInterface::GetExperience()
	{
		return ExperienceManager::GetSingleton()->GetExperience();
	}

	float ExperienceInterface::GetSkillCap(RE::ActorValue skill)
	{
		return PlayerSkillsEx::GetSkillCap(skill);
	}

	void ExperienceInterface::ShowNotification(const char* text, const char* status, const char* sound)
	{
		HUDMenuEx::ShowNotification(text, status, sound);
	}
}
