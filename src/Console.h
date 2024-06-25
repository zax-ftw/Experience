#pragma once

namespace ObScript
{
	using Command = RE::SCRIPT_FUNCTION;
	using ScriptData = Command::ScriptData;
	using ScriptParam = RE::SCRIPT_PARAMETER;
	using ParamType = RE::SCRIPT_PARAM_TYPE;

	bool AdvancePCLevel_Execute(const ScriptParam* paramInfo, ScriptData* scriptData, RE::TESObjectREFR* thisObj, RE::TESObjectREFR* containingObj, RE::Script* scriptObj, RE::ScriptLocals* locals, double& result, std::uint32_t& opcodeOffsetPtr);
	bool GetXPForNextLevel_Execute(const ScriptParam* paramInfo, ScriptData* scriptData, RE::TESObjectREFR* thisObj, RE::TESObjectREFR* containingObj, RE::Script* scriptObj, RE::ScriptLocals* locals, double& result, std::uint32_t& opcodeOffsetPtr);
	
	void AdvancePCLevel_Commit(Command* original);	
	void GetXPForNextLevel_Commit(Command* original);

	void Init();
}
