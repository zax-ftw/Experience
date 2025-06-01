#pragma once

class PlayerSkillsEx : public RE::PlayerCharacter::PlayerSkills
{
public:
	static constexpr uint32_t kSkillOffset = 6;
	static constexpr uint32_t kDataVersion = 2;

	void          InitSkills();
	void          EvaluateSkillData(uint32_t skillId);
	uint32_t      GetLevelData(float* points, float* pointsMax);
	void          GetSkillData(RE::ActorValue avId, float* level, float* points, float* pointsMax, uint32_t* legend);
	void          ModSkillPoints(RE::ActorValue avId, float points, void* source, bool power, bool recalc, bool silent);
	void          ModSkillLevels(RE::ActorValue avId, uint32_t levels);
	bool          CanLevelUp();

	static float GetSkillCap1(RE::ActorValue avId);
	static float GetSkillCap2(RE::ActorValue avId, float hard);
	static float GetBaseSkillCap(uint16_t level);

	static std::optional<int> ResolveAdvanceableSkillId(RE::ActorValue actorValue);

	static void Load(SKSE::SerializationInterface* intfc, uint32_t version, uint32_t length);
	static void Save(SKSE::SerializationInterface* intfc);
	static void Revert(SKSE::SerializationInterface* intfc);
	static void PostLoad();

	static void Install(SKSE::Trampoline& trampoline);

private:
	using Skill = Data::Skill;
	using SkillData = Data::SkillData;

	void          InitSkills_Hook();
	void          AdvanceLevel_Hook(bool addThreshold);
	void          GetSkillData_Hook(RE::ActorValue avId, float* level, float* points, float* pointsMax, uint32_t* legend);
	bool          CanLevelUp_Hook();

	static void ApplyRacials(RE::TESRace* race);
	static void UpdateSkillCaps();

	static void Update_V2(SKSE::SerializationInterface* intfc, uint32_t version);

	// members
	static inline float caps[Skill::kTotal];
};
static_assert(sizeof(PlayerSkillsEx) == 0x8);
