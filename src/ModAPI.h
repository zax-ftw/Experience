#pragma once

#include "ExperienceAPI.h"
#include "Utils/Singleton.h"


namespace Experience
{
	class ExperienceInterface : public InterfaceVersion1, 
		public ISingleton<ExperienceInterface>
	{
	public:
		friend class ISingleton<ExperienceInterface>;

		ApiVersion GetVersion() override { return ApiVersion::Current; };

		void  AddExperience(float points, bool meter) override;
		float GetExperience() override;
		float GetSkillCap(RE::ActorValue skill) override;
		void  ShowNotification(const char* text, const char* status, const char* sound) override;
	
	private:
		ExperienceInterface() = default;
		virtual ~ExperienceInterface() = default;
	};

}
