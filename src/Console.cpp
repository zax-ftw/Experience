#include "Console.h"

#include "Experience.h"
#include "Offsets.h"

using namespace RE;

bool ObScript::AdvancePCLevel_Execute(const ScriptParam* paramInfo, ScriptData* scriptData, TESObjectREFR* thisObj, TESObjectREFR* containingObj, Script* scriptObj, ScriptLocals* locals, double&, std::uint32_t& opcodeOffsetPtr)
{
	std::int32_t points;
	RE::Script::ParseParameters(paramInfo, scriptData, opcodeOffsetPtr, thisObj, containingObj, scriptObj, locals, &points);

	ExperienceManager::Experience experience;

	experience.points = points;
	experience.widget = 0;
	experience.source = nullptr;

	ExperienceManager::GetSingleton()->AddExperience(experience);

	return true;
}

bool ObScript::GetXPForNextLevel_Execute(const ScriptParam* paramInfo, ScriptData* scriptData, RE::TESObjectREFR* thisObj, RE::TESObjectREFR* containingObj, RE::Script* scriptObj, RE::ScriptLocals* locals, double& result, std::uint32_t& opcodeOffsetPtr)
{
	float points = 0.0f;

	auto player = PlayerCharacter::GetSingleton();
	if (auto skills = player->skills) {
		if (auto data = skills->data) {
			points = data->levelThreshold - data->xp;
		}
	}

	auto console = ConsoleLog::GetSingleton();
	console->Print("GetXPForNextLevel >> %0.2f", points);

	return true;
}

void ObScript::AdvancePCLevel_Commit(Command* original)
{
	Command cmd = *original;

//	cmd.longName = "";
//	cmd.shortName = "";
	cmd.helpString = "Give the player the given amount of character experience";
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

void ObScript::GetXPForNextLevel_Commit(Command* original) 
{
	Command cmd = *original;

	//	cmd.functionName = "";
	//	cmd.shortName = "";
	cmd.helpString = "Returns the amount of XP the player needs before reaching the next level";
	cmd.referenceFunction = false;
	cmd.numParams = 0;
	cmd.executeFunction = GetXPForNextLevel_Execute;
	cmd.editorFilter = false;

	//ScriptParam* param = new ScriptParam();
	//param->paramName = "Integer";
	//param->paramType = ParamType::kInt;
	//param->optional = false;

	//cmd.params = param;

	REL::safe_write((uintptr_t)original, &cmd, sizeof(cmd));
}

void ObScript::Init()
{
	Command* func1 = Command::LocateScriptCommand("AdvancePCLevel");
	if (func1) {
		AdvancePCLevel_Commit(func1);
	}

	Command* func2 = Command::LocateScriptCommand("GetXPForNextLevel");
	if (func2) {
		GetXPForNextLevel_Commit(func2);
	}
}
