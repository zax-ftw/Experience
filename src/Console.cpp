#include "Console.h"

#include "Experience.h"
#include "Offsets.h"

using namespace RE;

bool ObScript::ExtractArgs(const ScriptParam* paramInfo, ScriptData* scriptData, std::uint32_t& opcodeOffsetPtr, TESObjectREFR* thisObj, TESObjectREFR* containingObj, Script* scriptObj, ScriptLocals* locals, void* args1, void* args2)
{
	using func_t = decltype(&ObScript::ExtractArgs);
	REL::Relocation<func_t> func{ Offset::ObScript::ExtractArgs };
	return func(paramInfo, scriptData, opcodeOffsetPtr, thisObj, containingObj, scriptObj, locals, args1, args2);
}

bool ObScript::AdvancePCLevel_Execute(const ScriptParam* paramInfo, ScriptData* scriptData, TESObjectREFR* thisObj, TESObjectREFR* containingObj, Script* scriptObj, ScriptLocals* locals, double&, std::uint32_t& opcodeOffsetPtr)
{
	std::int32_t points;
	ObScript::ExtractArgs(paramInfo, scriptData, opcodeOffsetPtr, thisObj, containingObj, scriptObj, locals, &points, nullptr);

	ExperienceManager::Experience experience;

	experience.points = points;
	experience.widget = 0;
	experience.source = nullptr;

	ExperienceManager::GetSingleton().AddExperience(experience);

	return true;
}

void ObScript::AdvancePCLevel_Commit(Command* original)
{
	Command cmd = *original;

//	cmd.longName = "";
//	cmd.shortName = "";
	cmd.helpString = "Give the player the given amount of experience";
	cmd.referenceFunction = false;
	cmd.numParams = 1;
	cmd.executeFunction = AdvancePCLevel_Execute;
	cmd.editorFilter = false;

	ScriptParam* param = new ScriptParam();
	param->paramName = "Integer";
	param->paramType = ParamType::kInt;
	param->optional = false;

	cmd.params = param;

	REL::safe_write((uintptr_t)original, &cmd, sizeof(cmd));
}

void ObScript::Init()
{
	Command* func = Command::LocateScriptCommand("AdvancePCLevel");
	if (func) {
		AdvancePCLevel_Commit(func);
	}
}
