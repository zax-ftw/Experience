#pragma once

class PlayerSkillsEx : public RE::PlayerCharacter::PlayerSkills
{
public:
	static constexpr uint32_t kSkillOffset = 6;
	static constexpr uint32_t kDataVersion = 2;

	void     InitializeData();
	void     EvaluateSkillData(uint32_t skillId);
	uint32_t GetLevelInfo(float* points, float* pointsMax);
	void     GetSkillInfo(RE::ActorValue avId, float* level, float* points, float* pointsMax, uint32_t* legend);
	void     UseSkill(RE::ActorValue avId, float points, void* source, bool power, bool recalc, bool silent);
	void     IncrementSkill(RE::ActorValue avId, uint32_t levels);
	bool     IsReadyToLevelUp();

	static float GetSkillCap(RE::ActorValue avId);
	static float GetBaseSkillCap(uint16_t level);
	static bool IsSkillCapped(RE::ActorValue avId);

	static std::optional<int> ResolveAdvanceableSkillId(RE::ActorValue avId);

	static void Load(SKSE::SerializationInterface* intfc, uint32_t version, uint32_t length);
	static void Save(SKSE::SerializationInterface* intfc);
	static void Revert(SKSE::SerializationInterface* intfc);
	static void PostLoad();

	static void Install(SKSE::Trampoline& trampoline);

private:
	using Skill = Data::Skill;
	using SkillData = Data::SkillData;

	static void InitializeData_Hook(PlayerSkillsEx* skills);
	static void FinishLevelUp_Hook(PlayerSkillsEx* skills, bool addThreshold);
	static void GetSkillInfo_Hook(PlayerSkillsEx* skills, RE::ActorValue avId, float* level, float* points, float* pointsMax, uint32_t* legend);
	static void UseSkill_Hook(PlayerSkillsEx* skills, RE::ActorValue avId, float points, void* source, bool power, bool recalc, bool silent);
	static bool IsReadyToLevelUp_Hook(PlayerSkillsEx* skills);

	static void ApplyRacials(RE::TESRace* race);
	static void UpdateSkillCaps();

	static void Update_V2(SKSE::SerializationInterface* intfc, uint32_t version);

	// members
	static inline float caps[Skill::kTotal]; // zero initialized

	// hooked
	static inline REL::Relocation<decltype(InitializeData_Hook)> _InitializeData;
	static inline REL::Relocation<decltype(FinishLevelUp_Hook)> _FinishLevelUp;
	static inline REL::Relocation<decltype(GetSkillInfo_Hook)> _GetSkillInfo;
	static inline REL::Relocation<decltype(UseSkill_Hook)> _UseSkill;
	static inline REL::Relocation<decltype(IsReadyToLevelUp_Hook)> _IsReadyToLevelUp;
};
static_assert(sizeof(PlayerSkillsEx) == 0x8);
