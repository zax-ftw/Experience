#include "Serialization.h"

#include "Hooks/PlayerSkills.h"

namespace Serialization
{
	std::string DecodeTypeCode(std::uint32_t typeCode)
	{
		constexpr std::size_t SIZE = sizeof(std::uint32_t);

		std::string sig;
		sig.resize(SIZE);
		const char* iter = reinterpret_cast<char*>(&typeCode);
		for (std::size_t i = 0, j = SIZE - 1; i < SIZE; ++i, --j) {
			sig[j] = iter[i];
		}

		return sig;
	}

	void SaveCallback(SKSE::SerializationInterface* intfc)
	{
		PlayerSkillsEx::Save(intfc);

		logger::info("Finished saving data");
	}

	void LoadCallback(SKSE::SerializationInterface* intfc)
	{
		uint32_t type, version, length;

		while (intfc->GetNextRecordInfo(type, version, length)) {

			switch (type) {
			case kSkillData:
				PlayerSkillsEx::Load(intfc, version, length);
				break;
			case kSkillCaps:
				/* placeholder */
				break;
			default:
				logger::critical("Unrecognized record type in cosave ({})", DecodeTypeCode(type));
				break;
			}
		}
		logger::info("Finished loading data");
	}

	void RevertCallback(SKSE::SerializationInterface* intfc)
	{
		PlayerSkillsEx::Revert(intfc);

		logger::info("Finished reverting data");
	}
}
