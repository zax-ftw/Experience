#include "Hooks/PlayerSkills.h"

#include "Offsets.h"
#include "Settings.h"

#include "Skyrim/StatsMenu.h"

using namespace RE;

PlayerCharacter::PlayerSkills* PlayerSkillsEx::Ctor()
{
	using func_t = decltype(&PlayerSkillsEx::Ctor);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::Ctor };
	return func(this);
}

void PlayerSkillsEx::InitSkills()
{
	using func_t = decltype(&PlayerSkillsEx::InitSkills);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::InitSkills };
	return func(this);
}

void PlayerSkillsEx::EvaluateSkillData(uint32_t skillId)
{
	using func_t = decltype(&PlayerSkillsEx::EvaluateSkillData);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::EvaluateSkillData };
	return func(this, skillId);
}

uint32_t PlayerSkillsEx::GetLevelData(float* points, float* pointsMax)
{
	using func_t = decltype(&PlayerSkillsEx::GetLevelData);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::GetLevelData };
	return func(this, points, pointsMax);
}

void PlayerSkillsEx::GetSkillData(ActorValue avId, float* level, float* points, float* pointsMax, uint32_t* legend)
{
	using func_t = decltype(&PlayerSkillsEx::GetSkillData);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::GetSkillData };
	return func(this, avId, level, points, pointsMax, legend);
}

bool PlayerSkillsEx::CanLevelUp()
{
	using func_t = decltype(&PlayerSkillsEx::CanLevelUp);
	REL::Relocation<func_t> func{ Offset::PlayerSkills::CanLevelUp };
	return func(this);
}

std::optional<int> PlayerSkillsEx::ResolveAdvanceableSkillId(ActorValue actorValue)
{
	int skill = static_cast<int32_t>(actorValue) - kSkillOffset;
	if (skill >= Skill::kOneHanded && skill < Skill::kTotal) {
		return skill;
	}
	return std::nullopt;
}

float PlayerSkillsEx::GetSkillCap1(ActorValue avId)
{
	auto id = ResolveAdvanceableSkillId(avId);
	if (id) {
		return caps[*id];
	}
	return 0.0f;
}

float PlayerSkillsEx::GetSkillCap2(ActorValue avId, float hard)
{
	return std::min(GetSkillCap1(avId), hard);
}

float PlayerSkillsEx::GetBaseSkillCap(uint16_t level)
{
	Settings& settings = Settings::GetSingleton();
	float base = settings.GetSettingFloat("fSkillCapBase");
	float mult = settings.GetSettingFloat("fSkillCapMult");

	return base + (level * mult);
}

void PlayerSkillsEx::ApplyRacials(TESRace* race)
{
	logger::info("Applying racials for {}", race->GetFormEditorID());

	RACE_DATA& info = race->data;
	for (auto boost : info.skillBoosts) {

		auto skill = boost.skill;  // stl::enumeration<ActorValue, int>
		auto bonus = boost.bonus;  // uint8_t

		logger::info("{}: {}", skill.get(), bonus);

		auto id = ResolveAdvanceableSkillId(skill.get());
		if (id) { 
			caps[*id] += bonus; 
		}
	}
}
// InitSkills, AdvanceLevel
void PlayerSkillsEx::UpdateSkillCaps()
{
	logger::info("Updating skillcap data...");

	auto player = PlayerCharacter::GetSingleton();
	uint16_t level = player->GetLevel();

	float base = GetBaseSkillCap(level);
	std::fill(std::begin(caps), std::end(caps), base);

	Settings& settings = Settings::GetSingleton();
	if (settings.GetSettingBool("bUseRacialCaps")) {
		ApplyRacials(player->GetRace());
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
	case 1:
		{
			Update_V2(intfc, version);
			break;
		}
	case 2:
		{
			intfc->ReadRecordData(&caps, length);
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

PlayerCharacter::PlayerSkills* PlayerSkillsEx::PlayerSkills_Hook()
{
	PlayerSkills* skills = Ctor();
	return skills;
}
// new game, race change
void PlayerSkillsEx::InitSkills_Hook()
{
	float points, levelup;
	GetLevelData(&points, &levelup);

	logger::info("Initializing skills...");

	data->levelThreshold = levelup;

	for (int id = 0; id < Skill::kTotal; id++) {
		EvaluateSkillData(id);
	}
	UpdateSkillCaps();
}
// ConfirmLevelUpAttributeCallback, SetLevel, UpdateLevel
void PlayerSkillsEx::AdvanceLevel_Hook(bool addThreshold)
{
	AdvanceLevel(addThreshold);
	UpdateSkillCaps();

	auto strings = InterfaceStrings::GetSingleton();
	auto ui = UI::GetSingleton();

	if (ui->IsMenuOpen(strings->statsMenu)) {
		auto temp = ui->GetMenu(strings->statsMenu);
		StatsMenuEx* menu = static_cast<StatsMenuEx*>(temp.get());
		if (menu) {
			menu->UpdateSkillList();
		}
	}
}

// UpdateSkillList -> AS2_InitAnimatedSkillText
void PlayerSkillsEx::GetSkillDatHook(ActorValue avId, float* level, float* xp, float* next, uint32_t* legend)
{
	auto id = ResolveAdvanceableSkillId(avId);
	if (id) {
		auto player = PlayerCharacter::GetSingleton()->AsActorValueOwner();
		SkillData* skill = &data->skills[*id];
		if (skill) {
			*level = skill->level; // unused
			*xp = caps[*id] > player->GetBaseActorValue(avId) ? skill->xp : -100000.0f;
			*next = skill->levelThreshold;
			*legend = data->legendaryLevels[*id];
		}
	}
}

bool PlayerSkillsEx::CanLevelUp_Hook()
{
	if (CanLevelUp()) {
		auto player = PlayerCharacter::GetSingleton();
		Settings& settings = Settings::GetSingleton();

		int maxLevel = settings.GetSettingInt("iMaxPlayerLevel");

		if (player->GetLevel() < maxLevel)
			return true;
	}
	return false;
}

void SkillCap_GenCode(Xbyak::CodeGenerator& code, uintptr_t func, uintptr_t call)
{
	using namespace Xbyak::util;

	if (REL::Module::IsAE()) {  // AE

		Xbyak::Label skipLabel;

		code.movss(xmm1, xmm10);  // max - a2 (xmm6-xmm15 - non volatile)

		code.mov(rcx, esi);  // avId - a1
		code.mov(rax, call);
		
		code.call(rax);  // new max float on xmm0

		code.comiss(xmm8, xmm0);  // lvl, max (original line)

		code.jnb(skipLabel);
		code.jmp(ptr[rip]);
		code.dq(func + 0x89);

		code.L(skipLabel);
		code.jmp(ptr[rip]);
		code.dq(func + 0x361);

	} else { // SE and VR

		code.movss(xmm6, xmm0);  // lvl (xmm6-xmm15 - non volatile)
		code.movss(xmm1, xmm8);  // max - a2

		code.mov(rcx, esi);  // avId - a1

		code.mov(rax, call);

		code.call(rax);  // new max float on xmm0

		code.comiss(xmm6, xmm0);  // lvl, max (original line)

		code.jmp(ptr[rip]);
		code.dq(func + 0x58);
	}
}

void PlayerSkillsEx::Install(SKSE::Trampoline& trampoline)
{
	Xbyak::CodeGenerator code;
	SkillCap_GenCode(
		code, 
		Offset::PlayerSkills::ModSkillPoints.address(), 
		reinterpret_cast<uintptr_t>(&PlayerSkillsEx::GetSkillCap2));

	trampoline.write_branch<6>(
		Offset::PlayerSkills::ModSkillPoints.address() + OFFSET(0x51, 0x7F, 0x51), 
		trampoline.allocate(code));

	//trampoline.write_call<5>(Offset::PlayerCharacter::Ctor.address() + OFFSET(0xC3D, 0xECD), &PlayerSkillsEx::PlayerSkills_Hook);

	trampoline.write_call<5>(Offset::Main::sub_5B5490.address() + OFFSET(0x289, 0x303, 0x29E), &PlayerSkillsEx::InitSkills_Hook);  // MainMenu
	trampoline.write_call<5>(Offset::Main::sub_5B6DC0.address() + OFFSET(0xE5, 0xE5, 0xE5), &PlayerSkillsEx::InitSkills_Hook);                  // MainMenu
	trampoline.write_branch<5>(Offset::PlayerCharacter::InitActorValues.address() + OFFSET(0x1A, 0x1A, 0x1A), &PlayerSkillsEx::InitSkills_Hook);

	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x40, 0x40, 0x40), &PlayerSkillsEx::AdvanceLevel_Hook);
	trampoline.write_call<5>(Offset::PlayerSkills::SetLevel.address() + OFFSET(0x3E, 0x3E, 0x3E), &PlayerSkillsEx::AdvanceLevel_Hook);
	trampoline.write_call<5>(Offset::ConfirmLevelUpAttributeCallback::Run.address() + OFFSET(0xCE, 0xCE, 0xCE), &PlayerSkillsEx::AdvanceLevel_Hook);

	trampoline.write_call<5>(Offset::StatsMenu::UpdateSkillList.address() + OFFSET(0x103, 0x104, 0x13D), &PlayerSkillsEx::GetSkillDatHook);

	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x2A, 0x2A, 0x2A), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x48, 0x48, 0x48), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::PlayerCharacter::sub_69ABF0.address() + OFFSET(0x37, 0x37, 0x5E), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::StatsMenu::ProcessMessage.address() + OFFSET(0xF23, 0xFA7, 0x100E), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::TweenMenu::sub_8D16A0.address() + OFFSET(0x72, 0x72, 0x72), &PlayerSkillsEx::CanLevelUp_Hook);
	
	if (REL::Module::IsAE()) {
		uint8_t kDisableExperienceGain[] = { 0x66, 0x0F, 0xEF, 0xC9, 0x90, 0x90, 0x90, 0x90 };  // pxor xmm1, xmm1
		REL::safe_write(Offset::PlayerSkills::ModSkillPoints.address() + 0x2CF,
			std::span<uint8_t>(kDisableExperienceGain));
	} else { // SE and VR
		uint8_t kDisableExperienceGain[] = { 0x66, 0x0F, 0xEF, 0xC0, 0x90 };  // pxor xmm0, xmm0
		REL::safe_write(Offset::PlayerSkills::ModSkillPoints.address() + 0x22B,
			std::span<uint8_t>(kDisableExperienceGain));
	}

	uint8_t kDisableLevelUpMessage[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };  // level meter for skills
	REL::safe_write(Offset::HUDNotifications::Update.address() + OFFSET(0x2FC, 0x2FF, 0x2FC),
		std::span<uint8_t>(kDisableLevelUpMessage));
}
