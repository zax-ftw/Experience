#include "Hooks/PlayerSkills.h"

#include "Offsets.h"
#include "Settings.h"

#include "Skyrim/StatsMenu.h"

using namespace RE;

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
	return id ? caps[*id] : 0.0f;
}

float PlayerSkillsEx::GetSkillCap2(ActorValue avId, float hard)
{
	return std::min(GetSkillCap1(avId), hard);
}

float PlayerSkillsEx::GetBaseSkillCap(uint16_t level)
{
	auto settings = Settings::GetSingleton();
	float base = settings->GetValue<float>("fSkillCapBase");
	float mult = settings->GetValue<float>("fSkillCapMult");

	return base + (level * mult);
}

void PlayerSkillsEx::ApplyRacials(TESRace* race)
{
	logger::info("Applying racials for {}", race->GetFormEditorID());

	for (auto& [skill, bonus] : race->data.skillBoosts) {

		logger::info("{}: {}", skill.underlying(), bonus);

		auto id = ResolveAdvanceableSkillId(*skill);
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

// workaround for existing characters
void PlayerSkillsEx::PostLoad()
{
	if (caps[0] == 0.0f) {
		UpdateSkillCaps();
	}
}
// new game, race change
void PlayerSkillsEx::InitSkills_Hook()
{
	float points, levelup;
	auto  level = GetLevelData(&points, &levelup);

	logger::info("Initializing skills at level {}", level);

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
		auto ptr = ui->GetMenu(strings->statsMenu);
		StatsMenuEx* menu = static_cast<StatsMenuEx*>(ptr.get());
		if (menu) {
			menu->UpdateSkillList();
		}
	}
}

// UpdateSkillList -> AS2_InitAnimatedSkillText
void PlayerSkillsEx::GetSkillData_Hook(ActorValue avId, float* level, float* xp, float* next, uint32_t* legend)
{
	auto id = ResolveAdvanceableSkillId(avId);
	if (id) {
		auto player = PlayerCharacter::GetSingleton();
		SkillData* skill = &data->skills[*id];
		if (skill) {
			if (level) {
				*level = skill->level;  // unused
			}
			if (xp) {
				*xp = caps[*id] > player->GetBaseActorValue(avId) ? skill->xp : -100000.0f;
			}
			if (next) {
				*next = skill->levelThreshold;
			}
			if (legend) {
				*legend = data->legendaryLevels[*id];
			}
		}
	}
}

bool PlayerSkillsEx::CanLevelUp_Hook()
{
	if (CanLevelUp()) {
		auto player = PlayerCharacter::GetSingleton();
		auto settings = Settings::GetSingleton();

		int maxLevel = settings->GetValue<int>("iMaxPlayerLevel");

		if (player->GetLevel() < maxLevel)
			return true;
	}
	return false;
}

void SkillCap_GenCode(Xbyak::CodeGenerator& code, uintptr_t func, uintptr_t call)
{
	using namespace Xbyak::util;

#ifdef SKYRIM_SUPPORT_AE
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
#else
		code.movss(xmm6, xmm0);  // lvl (xmm6-xmm15 - non volatile)
		code.movss(xmm1, xmm8);  // max - a2

		code.mov(rcx, esi);  // avId - a1

		code.mov(rax, call);

		code.call(rax);  // new max float on xmm0

		code.comiss(xmm6, xmm0);  // lvl, max (original line)

		code.jmp(ptr[rip]);
		code.dq(func + 0x58);
#endif
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


	trampoline.write_call<5>(Offset::Main::sub_5B5490.address() + OFFSET(0x289, 0x303, 0x29E), &PlayerSkillsEx::InitSkills_Hook);
	trampoline.write_call<5>(Offset::Main::sub_5B6DC0.address() + OFFSET(0xE5, 0xE5, 0xE5), &PlayerSkillsEx::InitSkills_Hook);
	trampoline.write_branch<5>(Offset::PlayerCharacter::InitActorValues.address() + OFFSET(0x1A, 0x1A, 0x1A), &PlayerSkillsEx::InitSkills_Hook);

	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x40, 0x40, 0x40), &PlayerSkillsEx::AdvanceLevel_Hook);
	trampoline.write_call<5>(Offset::PlayerSkills::SetLevel.address() + OFFSET(0x3E, 0x3E, 0x3E), &PlayerSkillsEx::AdvanceLevel_Hook);
	trampoline.write_call<5>(Offset::ConfirmLevelUpAttributeCallback::Run.address() + OFFSET(0xCE, 0xCE, 0xCE), &PlayerSkillsEx::AdvanceLevel_Hook);

	trampoline.write_call<5>(Offset::StatsMenu::UpdateSkillList.address() + OFFSET(0x103, 0x104, 0x13D), &PlayerSkillsEx::GetSkillData_Hook);
	trampoline.write_call<5>(Offset::TrainingMenu::SetSkillXpPercent.address() + OFFSET(0x87, 0x87, 0x87), &PlayerSkillsEx::GetSkillData_Hook);
	REL::safe_fill(Offset::TrainingMenu::SetSkillXpPercent.address() + OFFSET(0x44, 0x44, 0x44), REL::NOP, 0x2);

	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x2A, 0x2A, 0x2A), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::Console::UpdateLevel.address() + OFFSET(0x48, 0x48, 0x48), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::PlayerCharacter::StopWaitSleep.address() + OFFSET(0x37, 0x37, 0x5E), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::StatsMenu::ProcessMessage.address() + OFFSET(0xF23, 0xFA7, 0x100E), &PlayerSkillsEx::CanLevelUp_Hook);
	trampoline.write_call<5>(Offset::TweenMenu::sub_8D16A0.address() + OFFSET(0x72, 0x72, 0x72), &PlayerSkillsEx::CanLevelUp_Hook);
	
#ifdef SKYRIM_SUPPORT_AE
		uint8_t kDisableExperienceGain[] = { 0x66, 0x0F, 0xEF, 0xC9, 0x90, 0x90, 0x90, 0x90 };  // pxor xmm1, xmm1
		REL::safe_write(Offset::PlayerSkills::ModSkillPoints.address() + 0x2CF,
			std::span<uint8_t>(kDisableExperienceGain));
#else
		uint8_t kDisableExperienceGain[] = { 0x66, 0x0F, 0xEF, 0xC0, 0x90 };  // pxor xmm0, xmm0
		REL::safe_write(Offset::PlayerSkills::ModSkillPoints.address() + 0x22B,
			std::span<uint8_t>(kDisableExperienceGain));
#endif
	uint8_t kDisableLevelUpMessage[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };  // level meter for skills
	REL::safe_write(Offset::HUDNotifications::Update.address() + OFFSET(0x2FC, 0x2FF, 0x2FC),
		std::span<uint8_t>(kDisableLevelUpMessage));
}
