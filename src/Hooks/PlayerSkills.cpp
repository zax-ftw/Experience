#include "Hooks/PlayerSkills.h"

#include "Offsets.h"
#include "Settings.h"

#include "Skyrim/StatsMenu.h"

using namespace RE;

void PlayerSkillsEx::InitializeData()
{
	using func_t = decltype(&PlayerSkillsEx::InitializeData);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::InitializeData };
	return func(this);
}

void PlayerSkillsEx::EvaluateSkillData(uint32_t skillId)
{
	using func_t = decltype(&PlayerSkillsEx::EvaluateSkillData);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::EvaluateSkillData };
	return func(this, skillId);
}

uint32_t PlayerSkillsEx::GetLevelInfo(float* points, float* pointsMax)
{
	using func_t = decltype(&PlayerSkillsEx::GetLevelInfo);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::GetLevelInfo };
	return func(this, points, pointsMax);
}

void PlayerSkillsEx::GetSkillInfo(ActorValue avId, float* level, float* points, float* pointsMax, uint32_t* legend)
{
	using func_t = decltype(&PlayerSkillsEx::GetSkillInfo);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::GetSkillInfo };
	return func(this, avId, level, points, pointsMax, legend);
}

bool PlayerSkillsEx::IsReadyToLevelUp()
{
	using func_t = decltype(&PlayerSkillsEx::IsReadyToLevelUp);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::IsReadyToLevelUp };
	return func(this);
}

std::optional<int> PlayerSkillsEx::ResolveAdvanceableSkillId(ActorValue avId)
{
	int skill = static_cast<int32_t>(avId) - kSkillOffset;
	if (skill >= Skill::kOneHanded && skill < Skill::kTotal) {
		return skill;
	}
	return std::nullopt;
}

float PlayerSkillsEx::GetSkillCap(ActorValue avId)
{
	auto id = ResolveAdvanceableSkillId(avId);
	return id ? caps[*id] : std::numeric_limits<float>::max();
}

float PlayerSkillsEx::GetBaseSkillCap(uint16_t level)
{
	auto settings = Settings::GetSingleton();
	float base = settings->GetValue<float>("fSkillCapBase");
	float mult = settings->GetValue<float>("fSkillCapMult");

	return base + (level * mult);
}

bool PlayerSkillsEx::IsSkillCapped(ActorValue avId)
{
	if (avId != ActorValue::kNone) {
		auto player = PlayerCharacter::GetSingleton();
		if (player->GetBaseActorValue(avId) >= GetSkillCap(avId)) {
			return true;
		}
	}
	return false;
}

void PlayerSkillsEx::ApplyRacials(TESRace* race)
{
	logger::info("Applying racials for {}", race->GetFormEditorID());

	for (auto& [skill, bonus] : race->data.skillBoosts) {

		logger::info("{}: {}", *skill, bonus);

		auto id = ResolveAdvanceableSkillId(*skill);
		if (id) {
			caps[*id] += bonus;
		}
	}
}

void PlayerSkillsEx::UpdateSkillCaps()
{
	logger::info("Updating skillcap data...");

	auto player = PlayerCharacter::GetSingleton();
	auto level = player->GetLevel();

	float base = GetBaseSkillCap(level);
	std::fill(std::begin(caps), std::end(caps), base);

	auto settings = Settings::GetSingleton();
	if (settings->GetValue<bool>("bUseRacialCaps")) {

		if (auto race = player->GetRace()) {
			ApplyRacials(race);
		}	
	}
}

void PlayerSkillsEx::Update_V2(SKSE::SerializationInterface* intfc, uint32_t version)
{
	uint32_t data[Skill::kTotal];
	intfc->ReadRecordData(&data, sizeof(data));

	std::transform(std::begin(data), std::end(data), std::begin(caps),
		[](uint32_t i) { return static_cast<float>(i); });

	logger::info("Migrated data from v{} to v{}", version, kDataVersion);
}

void PlayerSkillsEx::Load(SKSE::SerializationInterface* intfc, uint32_t version, uint32_t length)
{
	switch (version) {
	case kDataVersion:
		{
			intfc->ReadRecordData(&caps, length);
			break;
		}
	case 1:
		{
			Update_V2(intfc, version);
			break;
		}
	default:
		logger::critical("Unknown version: {}", version);
		break;
	}
}

void PlayerSkillsEx::Save(SKSE::SerializationInterface* intfc)
{
	if (!intfc->OpenRecord('DATA', kDataVersion)) {
		logger::error("Unable to open record to write cosave data");
		return;
	}
	intfc->WriteRecordData(caps, sizeof(caps));
}

void PlayerSkillsEx::Revert(SKSE::SerializationInterface*)
{
	memset(caps, 0, sizeof(caps));
}

// Workaround for existing characters
void PlayerSkillsEx::PostLoad()
{
	if (caps[0] == 0.0f) {
		UpdateSkillCaps();
	}
}

// New Game, Race Change
void PlayerSkillsEx::InitializeData_Hook(PlayerSkillsEx* skills)
{
	float points, levelup;
	auto  level = skills->GetLevelInfo(&points, &levelup);

	logger::info("Initializing skills at level {}", level);

	skills->data->levelThreshold = levelup;

	for (int id = 0; id < Skill::kTotal; id++) {
		skills->EvaluateSkillData(id);
	}
	UpdateSkillCaps();
}

// ConfirmLevelUpAttributeCallback, ForceLevel, UpdateLevel
void PlayerSkillsEx::FinishLevelUp_Hook(PlayerSkillsEx* skills, bool addThreshold)
{
	_FinishLevelUp(skills, addThreshold);
	UpdateSkillCaps();

	auto strings = InterfaceStrings::GetSingleton();
	auto ui = UI::GetSingleton();

	if (ui->IsMenuOpen(strings->statsMenu)) {
		auto ptr = ui->GetMenu(strings->statsMenu);
		StatsMenuEx* menu = static_cast<StatsMenuEx*>(ptr.get());
		if (menu) {
			menu->InitSkills();
		}
	}
}

// InitSkills -> AS2_InitAnimatedSkillText
void PlayerSkillsEx::GetSkillInfo_Hook(PlayerSkillsEx* skills, ActorValue avId, float* level, float* xp, float* next, uint32_t* legend)
{
	_GetSkillInfo(skills, avId, level, xp, next, legend);

	if (IsSkillCapped(avId)) {
		*xp = -100.0f;
	}
}

void PlayerSkillsEx::UseSkill_Hook(PlayerSkillsEx* skills, RE::ActorValue avId, float points, void* source, bool power, bool recalc, bool silent)
{
	if (!IsSkillCapped(avId)) {
		_UseSkill(skills, avId, points, source, power, recalc, silent);
	}
}

bool PlayerSkillsEx::IsReadyToLevelUp_Hook(PlayerSkillsEx* skills)
{
	if (_IsReadyToLevelUp(skills)) {

		auto player = PlayerCharacter::GetSingleton();
		auto settings = Settings::GetSingleton();

		int maxLevel = settings->GetValue<int>("iMaxPlayerLevel");

		if (player->GetLevel() < maxLevel)
			return true;
	}
	return false;
}

void PlayerSkillsEx::Install(SKSE::Trampoline& trampoline)
{
	_InitializeData = trampoline.write_call<5>(Offset::Main::PerformGameReset.address() + OFFSET(0x289, 0x303, 0x29E), InitializeData_Hook);
	trampoline.write_call<5>(Offset::Main::UpdatePlayer.address() + OFFSET(0xE5, 0xE5, 0xE5), InitializeData_Hook);
	trampoline.write_branch<5>(Offset::PlayerCharacter::InitValues.address() + OFFSET(0x1A, 0x1A, 0x1A), InitializeData_Hook);

	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x40, 0x40, 0x40), FinishLevelUp_Hook);
	trampoline.write_call<5>(Offset::PlayerSkills::ForceLevel.address() + OFFSET(0x3E, 0x3E, 0x3E), FinishLevelUp_Hook);
	_FinishLevelUp = trampoline.write_call<5>(Offset::ConfirmLevelUpAttributeCallback::Run.address() + OFFSET(0xCE, 0xCE, 0xCE), FinishLevelUp_Hook);

	_GetSkillInfo = trampoline.write_call<5>(Offset::StatsMenu::InitSkills.address() + OFFSET(0x103, 0x104, 0x13D), GetSkillInfo_Hook);
	trampoline.write_call<5>(Offset::TrainingMenu::UpdateSkillMeter.address() + OFFSET(0x87, 0x87, 0x87), GetSkillInfo_Hook);
	REL::safe_fill(Offset::TrainingMenu::UpdateSkillMeter.address() + OFFSET(0x44, 0x44, 0x44), REL::NOP, 0x2);

	_UseSkill = trampoline.write_call<5>(Offset::PlayerCharacter::UseSkill.address() + OFFSET(0x25, 0x25, 0x25), UseSkill_Hook);
	trampoline.write_call<5>(Offset::PlayerSkills::IncrementSkill.address() + OFFSET(0x99, 0x98, 0x99), UseSkill_Hook);

	_IsReadyToLevelUp = trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x2A, 0x2A, 0x2A), IsReadyToLevelUp_Hook);
	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x48, 0x48, 0x48), IsReadyToLevelUp_Hook);
	trampoline.write_call<5>(Offset::PlayerCharacter::WakeUp.address() + OFFSET(0x37, 0x37, 0x5E), IsReadyToLevelUp_Hook);
	trampoline.write_call<5>(Offset::StatsMenu::ProcessMessage.address() + OFFSET(0xF23, 0xFA7, 0x100E), IsReadyToLevelUp_Hook);
	trampoline.write_call<5>(Offset::TweenMenu::StartMenuOpenAnim.address() + OFFSET(0x72, 0x72, 0x72), IsReadyToLevelUp_Hook);

#ifdef SKYRIM_SUPPORT_AE
	uint8_t kDisableExperienceGain[] = { 0x66, 0x0F, 0xEF, 0xC9, 0x90, 0x90, 0x90, 0x90 };  // pxor xmm1, xmm1
#else
	uint8_t kDisableExperienceGain[] = { 0x66, 0x0F, 0xEF, 0xC0, 0x90 };  // pxor xmm0, xmm0
#endif
	REL::safe_write<uint8_t>(Offset::PlayerSkills::UseSkill.address() + OFFSET(0x22B, 0x2CF, 0x22B), kDisableExperienceGain);

	uint8_t kDisableLevelUpMessage[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };  // level meter for skills
	REL::safe_write<uint8_t>(Offset::HUDNotifications::Update.address() + OFFSET(0x2FC, 0x2FF, 0x2FC), kDisableLevelUpMessage);
}
