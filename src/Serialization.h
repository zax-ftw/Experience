#pragma once

namespace Serialization
{
	enum : uint32_t
	{
		kSkillData = 'DATA',
		kSkillCaps = 'CAPS'
	};

	void SaveCallback(SKSE::SerializationInterface* intfc);
	void LoadCallback(SKSE::SerializationInterface* intfc);
	void RevertCallback(SKSE::SerializationInterface* intfc);

}
