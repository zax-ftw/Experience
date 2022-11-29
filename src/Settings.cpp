#include "Settings.h"

using namespace SKSE;

int GetIntValue(CSimpleIniA& ini, const char* section, const char* setting, int _default)
{
	return ini.GetLongValue(section, setting, _default);
}

bool GetBoolValue(CSimpleIniA& ini, const char* section, const char* setting, bool _default)
{
	return ini.GetBoolValue(section, setting, _default);
}

float GetFloatValue(CSimpleIniA& ini, const char* section, const char* setting, float _default)
{
	return static_cast<float>(ini.GetDoubleValue(section, setting, _default));
}

auto GetStringValue(CSimpleIniA& ini, const char* section, const char* setting, const char* _default)
{
	return ini.GetValue(section, setting, _default);
}

void SetIntValue(CSimpleIniA& ini, const char* section, const char* setting, int value)
{
	ini.SetLongValue(section, setting, value);
}

void SetBoolValue(CSimpleIniA& ini, const char* section, const char* setting, bool value)
{
	ini.SetLongValue(section, setting, value);
}

void SetFloatValue(CSimpleIniA& ini, const char* section, const char* setting, float value)
{
	ini.SetDoubleValue(section, setting, value);
}

void SetStringValue(CSimpleIniA& ini, const char* section, const char* setting, const char* value)
{
	ini.SetValue(section, setting, value);
}

Settings::Settings()
{
	auto plugin = PluginDeclaration::GetSingleton()->GetName();
	path = fmt::format("Data/SKSE/Plugins/{}.ini", plugin);
}

void Settings::LoadSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	if (ini.LoadFile(path.c_str()) == SI_FILE) {
		logger::warn("Failed to load file: {}", path);
		return;
	}

	// general
	data["bEnableKilling"] = GetBoolValue(ini, "General", "bEnableKilling", true);
	data["bEnableReading"] = GetBoolValue(ini, "General", "bEnableReading", true);
	data["bEnableSkillXP"] = GetBoolValue(ini, "General", "bEnableSkillXP", false);
	data["bForceLvlMeter"] = GetBoolValue(ini, "General", "bForceLvlMeter", false);
	data["bShowMessages"] = GetBoolValue(ini, "General", "bShowMessages", true);
	data["iMaxPlayerLevel"] = GetIntValue(ini, "General", "iMaxPlayerLevel", 255);
	data["fSkillCapBase"] = GetFloatValue(ini, "General", "fSkillCapBase", 18.0f);
	data["fSkillCapMult"] = GetFloatValue(ini, "General", "fSkillCapMult", 2.0f);
	data["bUseRacialCaps"] = GetBoolValue(ini, "General", "bUseRacialCaps", true);
	data["sMessageFormat"] = GetStringValue(ini, "General", "sMessageFormat", "{:+d} XP");

	// questing
	data["iXPQuestNone"] = GetIntValue(ini, "Questing", "iXPQuestNone", 0);
	data["iXPQuestMain"] = GetIntValue(ini, "Questing", "iXPQuestMain", 75);
	data["iXPQuestCollege"] = GetIntValue(ini, "Questing", "iXPQuestCollege", 50);
	data["iXPQuestThieves"] = GetIntValue(ini, "Questing", "iXPQuestThieves", 50);
	data["iXPQuestBrotherhood"] = GetIntValue(ini, "Questing", "iXPQuestBrotherhood", 50);
	data["iXPQuestCompanions"] = GetIntValue(ini, "Questing", "iXPQuestCompanions", 50);
	data["iXPQuestMisc"] = GetIntValue(ini, "Questing", "iXPQuestMisc", 0);
	data["iXPQuestDaedric"] = GetIntValue(ini, "Questing", "iXPQuestDaedric", 75);
	data["iXPQuestSide"] = GetIntValue(ini, "Questing", "iXPQuestSide", 50);
	data["iXPQuestCivilWar"] = GetIntValue(ini, "Questing", "iXPQuestCivilWar", 75);
	data["iXPQuestDawnguard"] = GetIntValue(ini, "Questing", "iXPQuestDawnguard", 50);
	data["iXPQuestDragonborn"] = GetIntValue(ini, "Questing", "iXPQuestDragonborn", 75);
	data["iXPObjectives"] = GetIntValue(ini, "Questing", "iXPObjectives", 10);

	// exploring
	int iXPDiscDefault = GetIntValue(ini, "Exploring", "iXPDiscDefault", 10);

	data["iXPDiscDefault"] = iXPDiscDefault;
	data["iXPDiscCity"] = GetIntValue(ini, "Exploring", "iXPDiscCity", iXPDiscDefault);
	data["iXPDiscTown"] = GetIntValue(ini, "Exploring", "iXPDiscTown", iXPDiscDefault);
	data["iXPDiscSettlement"] = GetIntValue(ini, "Exploring", "iXPDiscSettlement", iXPDiscDefault);
	data["iXPDiscCave"] = GetIntValue(ini, "Exploring", "iXPDiscCave", iXPDiscDefault);
	data["iXPDiscCamp"] = GetIntValue(ini, "Exploring", "iXPDiscCamp", iXPDiscDefault);
	data["iXPDiscFort"] = GetIntValue(ini, "Exploring", "iXPDiscFort", iXPDiscDefault);
	data["iXPDiscNordicRuin"] = GetIntValue(ini, "Exploring", "iXPDiscNordicRuin", iXPDiscDefault);
	data["iXPDiscDwemerRuin"] = GetIntValue(ini, "Exploring", "iXPDiscDwemerRuin", iXPDiscDefault);
	data["iXPDiscShipwreck"] = GetIntValue(ini, "Exploring", "iXPDiscShipwreck", iXPDiscDefault);
	data["iXPDiscGrove"] = GetIntValue(ini, "Exploring", "iXPDiscGrove", iXPDiscDefault);
	data["iXPDiscLandmark"] = GetIntValue(ini, "Exploring", "iXPDiscLandmark", iXPDiscDefault);
	data["iXPDiscDragonLair"] = GetIntValue(ini, "Exploring", "iXPDiscDragonLair", iXPDiscDefault);
	data["iXPDiscFarm"] = GetIntValue(ini, "Exploring", "iXPDiscFarm", iXPDiscDefault);
	data["iXPDiscWoodMill"] = GetIntValue(ini, "Exploring", "iXPDiscWoodMill", iXPDiscDefault);
	data["iXPDiscMine"] = GetIntValue(ini, "Exploring", "iXPDiscMine", iXPDiscDefault);
	data["iXPDiscMilitaryCamp"] = GetIntValue(ini, "Exploring", "iXPDiscMilitaryCamp", iXPDiscDefault);
	data["iXPDiscDoomstone"] = GetIntValue(ini, "Exploring", "iXPDiscDoomstone", iXPDiscDefault);
	data["iXPDiscWheatMill"] = GetIntValue(ini, "Exploring", "iXPDiscWheatMill", iXPDiscDefault);
	data["iXPDiscSmelter"] = GetIntValue(ini, "Exploring", "iXPDiscSmelter", iXPDiscDefault);
	data["iXPDiscStable"] = GetIntValue(ini, "Exploring", "iXPDiscStable", iXPDiscDefault);
	data["iXPDiscImperialTower"] = GetIntValue(ini, "Exploring", "iXPDiscImperialTower", iXPDiscDefault);
	data["iXPDiscClearing"] = GetIntValue(ini, "Exploring", "iXPDiscClearing", iXPDiscDefault);
	data["iXPDiscPass"] = GetIntValue(ini, "Exploring", "iXPDiscPass", iXPDiscDefault);
	data["iXPDiscAltar"] = GetIntValue(ini, "Exploring", "iXPDiscAltar", iXPDiscDefault);
	data["iXPDiscRock"] = GetIntValue(ini, "Exploring", "iXPDiscRock", iXPDiscDefault);
	data["iXPDiscLighthouse"] = GetIntValue(ini, "Exploring", "iXPDiscLighthouse", iXPDiscDefault);
	data["iXPDiscOrcStronghold"] = GetIntValue(ini, "Exploring", "iXPDiscOrcStronghold", iXPDiscDefault);
	data["iXPDiscGiantCamp"] = GetIntValue(ini, "Exploring", "iXPDiscGiantCamp", iXPDiscDefault);
	data["iXPDiscShack"] = GetIntValue(ini, "Exploring", "iXPDiscShack", iXPDiscDefault);
	data["iXPDiscNordicTower"] = GetIntValue(ini, "Exploring", "iXPDiscNordicTower", iXPDiscDefault);
	data["iXPDiscNordicDwelling"] = GetIntValue(ini, "Exploring", "iXPDiscNordicDwelling", iXPDiscDefault);
	data["iXPDiscDocks"] = GetIntValue(ini, "Exploring", "iXPDiscDocks", iXPDiscDefault);
	data["iXPDiscDaedricShrine"] = GetIntValue(ini, "Exploring", "iXPDiscDaedricShrine", iXPDiscDefault);
	data["iXPDiscCastle"] = GetIntValue(ini, "Exploring", "iXPDiscCastle", iXPDiscDefault);
	//data["iXPDiscCapitol"] = GetIntValue(ini, "Exploring", "iXPDiscCapitol", iXPDiscDefault);
	//data["iXPDiscMiraakTemple"] = GetIntValue(ini, "Exploring", "iXPDiscMiraakTemple", iXPDiscDefault);
	//data["iXPDiscRavenRock"] = GetIntValue(ini, "Exploring", "iXPDiscRavenRock", iXPDiscDefault);
	//data["iXPDiscStandingStone"] = GetIntValue(ini, "Exploring", "iXPDiscStandingStone", iXPDiscDefault);
	//data["iXPDiscTelvanniTower"] = GetIntValue(ini, "Exploring", "iXPDiscTelvanniTower", iXPDiscDefault);
	//data["iXPDiscCastleKarstaag"] = GetIntValue(ini, "Exploring", "iXPDiscCastleKarstaag", iXPDiscDefault);

	// clearing
	int iXPClearDefault = GetIntValue(ini, "Clearing", "iXPClearDefault", 30);

	data["iXPClearDefault"] = iXPClearDefault;
	data["iXPClearCity"] = GetIntValue(ini, "Clearing", "iXPClearCity", iXPClearDefault);
	data["iXPClearTown"] = GetIntValue(ini, "Clearing", "iXPClearTown", iXPClearDefault);
	data["iXPClearSettlement"] = GetIntValue(ini, "Clearing", "iXPClearSettlement", iXPClearDefault);
	data["iXPClearCave"] = GetIntValue(ini, "Clearing", "iXPClearCave", iXPClearDefault);
	data["iXPClearCamp"] = GetIntValue(ini, "Clearing", "iXPClearCamp", iXPClearDefault);
	data["iXPClearFort"] = GetIntValue(ini, "Clearing", "iXPClearFort", iXPClearDefault);
	data["iXPClearNordicRuin"] = GetIntValue(ini, "Clearing", "iXPClearNordicRuin", iXPClearDefault);
	data["iXPClearDwemerRuin"] = GetIntValue(ini, "Clearing", "iXPClearDwemerRuin", iXPClearDefault);
	data["iXPClearShipwreck"] = GetIntValue(ini, "Clearing", "iXPClearShipwreck", iXPClearDefault);
	data["iXPClearGrove"] = GetIntValue(ini, "Clearing", "iXPClearGrove", iXPClearDefault);
	data["iXPClearLandmark"] = GetIntValue(ini, "Clearing", "iXPClearLandmark", iXPClearDefault);
	data["iXPClearDragonLair"] = GetIntValue(ini, "Clearing", "iXPClearDragonLair", iXPClearDefault);
	data["iXPClearFarm"] = GetIntValue(ini, "Clearing", "iXPClearFarm", iXPClearDefault);
	data["iXPClearWoodMill"] = GetIntValue(ini, "Clearing", "iXPClearWoodMill", iXPClearDefault);
	data["iXPClearMine"] = GetIntValue(ini, "Clearing", "iXPClearMine", iXPClearDefault);
	data["iXPClearMilitaryCamp"] = GetIntValue(ini, "Exploring", "iXPClearMilitaryCamp", iXPClearDefault);
	data["iXPClearDoomstone"] = GetIntValue(ini, "Clearing", "iXPClearDoomstone", iXPClearDefault);
	data["iXPClearWheatMill"] = GetIntValue(ini, "Clearing", "iXPClearWheatMill", iXPClearDefault);
	data["iXPClearSmelter"] = GetIntValue(ini, "Clearing", "iXPClearSmelter", iXPClearDefault);
	data["iXPClearStable"] = GetIntValue(ini, "Clearing", "iXPClearStable", iXPClearDefault);
	data["iXPClearImperialTower"] = GetIntValue(ini, "Clearing", "iXPClearImperialTower", iXPClearDefault);
	data["iXPClearClearing"] = GetIntValue(ini, "Clearing", "iXPClearClearing", iXPClearDefault);
	data["iXPClearPass"] = GetIntValue(ini, "Clearing", "iXPClearPass", iXPClearDefault);
	data["iXPClearAltar"] = GetIntValue(ini, "Clearing", "iXPClearAltar", iXPClearDefault);
	data["iXPClearRock"] = GetIntValue(ini, "Clearing", "iXPClearRock", iXPClearDefault);
	data["iXPClearLighthouse"] = GetIntValue(ini, "Clearing", "iXPClearLighthouse", iXPClearDefault);
	data["iXPClearOrcStronghold"] = GetIntValue(ini, "Clearing", "iXPClearOrcStronghold", iXPClearDefault);
	data["iXPClearGiantCamp"] = GetIntValue(ini, "Clearing", "iXPClearGiantCamp", iXPClearDefault);
	data["iXPClearShack"] = GetIntValue(ini, "Clearing", "iXPClearShack", iXPClearDefault);
	data["iXPClearNordicTower"] = GetIntValue(ini, "Clearing", "iXPClearNordicTower", iXPClearDefault);
	data["iXPClearNordicDwelling"] = GetIntValue(ini, "Clearing", "iXPClearNordicDwelling", iXPClearDefault);
	data["iXPClearDocks"] = GetIntValue(ini, "Clearing", "iXPClearDocks", iXPClearDefault);
	data["iXPClearDaedricShrine"] = GetIntValue(ini, "Clearing", "iXPClearDaedricShrine", iXPClearDefault);
	data["iXPClearCastle"] = GetIntValue(ini, "Clearing", "iXPClearCastle", iXPClearDefault);
	//data["iXPClearCapitol"] = GetIntValue(ini, "Clearing", "iXPClearCapitol", iXPClearDefault);
	//data["iXPClearMiraakTemple"] = GetIntValue(ini, "Clearing", "iXPClearMiraakTemple", iXPClearDefault);
	//data["iXPClearRavenRock"] = GetIntValue(ini, "Clearing", "iXPClearRavenRock", iXPClearDefault);
	//data["iXPClearStandingStone"] = GetIntValue(ini, "Clearing", "iXPClearStandingStone", iXPClearDefault);
	//data["iXPClearTelvanniTower"] = GetIntValue(ini, "Clearing", "iXPClearTelvanniTower", iXPClearDefault);
	//data["iXPClearCastleKarstaag"] = GetIntValue(ini, "Clearing", "iXPClearCastleKarstaag", iXPClearDefault);

	// killing
	data["iXPLevelRange"] = GetIntValue(ini, "Killing", "iXPLevelRange", 20);
	data["fXPKillingMult"] = GetFloatValue(ini, "Killing", "fXPKillingMult", 1.0f);
	data["fXPGroupFactor"] = GetFloatValue(ini, "Killing", "fXPGroupFactor", 0.2f);

	// reading
	data["fXPReadingMult"] = GetFloatValue(ini, "Reading", "fXPReadingMult", 2.0f);

	logger::info("Settings loaded");
}

void Settings::SaveSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	// general
	SetBoolValue(ini, "General", "bEnableKilling", GetSettingBool("bEnableKilling"));
	SetBoolValue(ini, "General", "bEnableReading", GetSettingBool("bEnableReading"));
	SetBoolValue(ini, "General", "bEnableSkillXP", GetSettingBool("bEnableSkillXP"));
	SetBoolValue(ini, "General", "bForceLvlMeter", GetSettingBool("bForceLvlMeter"));
	SetBoolValue(ini, "General", "bShowMessages", GetSettingBool("bShowMessages"));
	SetIntValue(ini, "General", "iMaxPlayerLevel", GetSettingInt("iMaxPlayerLevel"));
	SetFloatValue(ini, "General", "fSkillCapBase", GetSettingFloat("fSkillCapBase"));
	SetFloatValue(ini, "General", "fSkillCapMult", GetSettingFloat("fSkillCapMult"));
	SetBoolValue(ini, "General", "bUseRacialCaps", GetSettingBool("bUseRacialCaps"));
	SetStringValue(ini, "General", "sMessageFormat", GetSettingString("sMessageFormat").c_str());

	// questing
	SetIntValue(ini, "Questing", "iXPQuestNone", GetSettingInt("iXPQuestNone"));
	SetIntValue(ini, "Questing", "iXPQuestMain", GetSettingInt("iXPQuestMain"));
	SetIntValue(ini, "Questing", "iXPQuestCollege", GetSettingInt("iXPQuestCollege"));
	SetIntValue(ini, "Questing", "iXPQuestThieves", GetSettingInt("iXPQuestThieves"));
	SetIntValue(ini, "Questing", "iXPQuestBrotherhood", GetSettingInt("iXPQuestBrotherhood"));
	SetIntValue(ini, "Questing", "iXPQuestCompanions", GetSettingInt("iXPQuestCompanions"));
	SetIntValue(ini, "Questing", "iXPQuestMisc", GetSettingInt("iXPQuestMisc"));
	SetIntValue(ini, "Questing", "iXPQuestDaedric", GetSettingInt("iXPQuestDaedric"));
	SetIntValue(ini, "Questing", "iXPQuestSide", GetSettingInt("iXPQuestSide"));
	SetIntValue(ini, "Questing", "iXPQuestCivilWar", GetSettingInt("iXPQuestCivilWar"));
	SetIntValue(ini, "Questing", "iXPQuestDawnguard", GetSettingInt("iXPQuestDawnguard"));
	SetIntValue(ini, "Questing", "iXPQuestDragonborn", GetSettingInt("iXPQuestDragonborn"));
	SetIntValue(ini, "Questing", "iXPObjectives", GetSettingInt("iXPObjectives"));

	// exploring
	SetIntValue(ini, "Exploring", "iXPDiscCity", GetSettingInt("iXPDiscCity"));
	SetIntValue(ini, "Exploring", "iXPDiscTown", GetSettingInt("iXPDiscTown"));
	SetIntValue(ini, "Exploring", "iXPDiscSettlement", GetSettingInt("iXPDiscSettlement"));
	SetIntValue(ini, "Exploring", "iXPDiscCave", GetSettingInt("iXPDiscCave"));
	SetIntValue(ini, "Exploring", "iXPDiscCamp", GetSettingInt("iXPDiscCamp"));
	SetIntValue(ini, "Exploring", "iXPDiscFort", GetSettingInt("iXPDiscFort"));
	SetIntValue(ini, "Exploring", "iXPDiscNordicRuin", GetSettingInt("iXPDiscNordicRuin"));
	SetIntValue(ini, "Exploring", "iXPDiscDwemerRuin", GetSettingInt("iXPDiscDwemerRuin"));
	SetIntValue(ini, "Exploring", "iXPDiscShipwreck", GetSettingInt("iXPDiscShipwreck"));
	SetIntValue(ini, "Exploring", "iXPDiscGrove", GetSettingInt("iXPDiscGrove"));
	SetIntValue(ini, "Exploring", "iXPDiscLandmark", GetSettingInt("iXPDiscLandmark"));
	SetIntValue(ini, "Exploring", "iXPDiscDragonLair", GetSettingInt("iXPDiscDragonLair"));
	SetIntValue(ini, "Exploring", "iXPDiscFarm", GetSettingInt("iXPDiscFarm"));
	SetIntValue(ini, "Exploring", "iXPDiscWoodMill", GetSettingInt("iXPDiscWoodMill"));
	SetIntValue(ini, "Exploring", "iXPDiscMine", GetSettingInt("iXPDiscMine"));
	SetIntValue(ini, "Exploring", "iXPDiscMilitaryCamp", GetSettingInt("iXPDiscMilitaryCamp"));
	SetIntValue(ini, "Exploring", "iXPDiscDoomstone", GetSettingInt("iXPDiscDoomstone"));
	SetIntValue(ini, "Exploring", "iXPDiscWheatMill", GetSettingInt("iXPDiscWheatMill"));
	SetIntValue(ini, "Exploring", "iXPDiscSmelter", GetSettingInt("iXPDiscSmelter"));
	SetIntValue(ini, "Exploring", "iXPDiscStable", GetSettingInt("iXPDiscStable"));
	SetIntValue(ini, "Exploring", "iXPDiscImperialTower", GetSettingInt("iXPDiscImperialTower"));
	SetIntValue(ini, "Exploring", "iXPDiscClearing", GetSettingInt("iXPDiscClearing"));
	SetIntValue(ini, "Exploring", "iXPDiscPass", GetSettingInt("iXPDiscPass"));
	SetIntValue(ini, "Exploring", "iXPDiscAltar", GetSettingInt("iXPDiscAltar"));
	SetIntValue(ini, "Exploring", "iXPDiscRock", GetSettingInt("iXPDiscRock"));
	SetIntValue(ini, "Exploring", "iXPDiscLighthouse", GetSettingInt("iXPDiscLighthouse"));
	SetIntValue(ini, "Exploring", "iXPDiscOrcStronghold", GetSettingInt("iXPDiscOrcStronghold"));
	SetIntValue(ini, "Exploring", "iXPDiscGiantCamp", GetSettingInt("iXPDiscGiantCamp"));
	SetIntValue(ini, "Exploring", "iXPDiscShack", GetSettingInt("iXPDiscShack"));
	SetIntValue(ini, "Exploring", "iXPDiscNordicTower", GetSettingInt("iXPDiscNordicTower"));
	SetIntValue(ini, "Exploring", "iXPDiscNordicDwelling", GetSettingInt("iXPDiscNordicDwelling"));
	SetIntValue(ini, "Exploring", "iXPDiscDocks", GetSettingInt("iXPDiscDocks"));
	SetIntValue(ini, "Exploring", "iXPDiscDaedricShrine", GetSettingInt("iXPDiscDaedricShrine"));
	SetIntValue(ini, "Exploring", "iXPDiscCastle", GetSettingInt("iXPDiscCastle"));
	//SetIntValue(ini, "Exploring", "iXPDiscCapitol", GetSettingInt("iXPDiscCapitol"));
	//SetIntValue(ini, "Exploring", "iXPDiscMiraakTemple", GetSettingInt("iXPDiscMiraakTemple"));
	//SetIntValue(ini, "Exploring", "iXPDiscRavenRock", GetSettingInt("iXPDiscRavenRock"));
	//SetIntValue(ini, "Exploring", "iXPDiscStandingStone", GetSettingInt("iXPDiscStandingStone"));
	//SetIntValue(ini, "Exploring", "iXPDiscTelvanniTower", GetSettingInt("iXPDiscTelvanniTower"));
	//SetIntValue(ini, "Exploring", "iXPDiscCastleKarstaag", GetSettingInt("iXPDiscCastleKarstaag"));
	SetIntValue(ini, "Exploring", "iXPDiscDefault", GetSettingInt("iXPDiscDefault"));

	// clearing
	SetIntValue(ini, "Clearing", "iXPClearCity", GetSettingInt("iXPClearCity"));
	SetIntValue(ini, "Clearing", "iXPClearTown", GetSettingInt("iXPClearTown"));
	SetIntValue(ini, "Clearing", "iXPClearSettlement", GetSettingInt("iXPClearSettlement"));
	SetIntValue(ini, "Clearing", "iXPClearCave", GetSettingInt("iXPClearCave"));
	SetIntValue(ini, "Clearing", "iXPClearCamp", GetSettingInt("iXPClearCamp"));
	SetIntValue(ini, "Clearing", "iXPClearFort", GetSettingInt("iXPClearFort"));
	SetIntValue(ini, "Clearing", "iXPClearNordicRuin", GetSettingInt("iXPClearNordicRuin"));
	SetIntValue(ini, "Clearing", "iXPClearDwemerRuin", GetSettingInt("iXPClearDwemerRuin"));
	SetIntValue(ini, "Clearing", "iXPClearShipwreck", GetSettingInt("iXPClearShipwreck"));
	SetIntValue(ini, "Clearing", "iXPClearGrove", GetSettingInt("iXPClearGrove"));
	SetIntValue(ini, "Clearing", "iXPClearLandmark", GetSettingInt("iXPClearLandmark"));
	SetIntValue(ini, "Clearing", "iXPClearDragonLair", GetSettingInt("iXPClearDragonLair"));
	SetIntValue(ini, "Clearing", "iXPClearFarm", GetSettingInt("iXPClearFarm"));
	SetIntValue(ini, "Clearing", "iXPClearWoodMill", GetSettingInt("iXPClearWoodMill"));
	SetIntValue(ini, "Clearing", "iXPClearMine", GetSettingInt("iXPClearMine"));
	SetIntValue(ini, "Clearing", "iXPClearMilitaryCamp", GetSettingInt("iXPClearMilitaryCamp"));
	SetIntValue(ini, "Clearing", "iXPClearDoomstone", GetSettingInt("iXPClearDoomstone"));
	SetIntValue(ini, "Clearing", "iXPClearWheatMill", GetSettingInt("iXPClearWheatMill"));
	SetIntValue(ini, "Clearing", "iXPClearSmelter", GetSettingInt("iXPClearSmelter"));
	SetIntValue(ini, "Clearing", "iXPClearStable", GetSettingInt("iXPClearStable"));
	SetIntValue(ini, "Clearing", "iXPClearImperialTower", GetSettingInt("iXPClearImperialTower"));
	SetIntValue(ini, "Clearing", "iXPClearClearing", GetSettingInt("iXPClearClearing"));
	SetIntValue(ini, "Clearing", "iXPClearPass", GetSettingInt("iXPClearPass"));
	SetIntValue(ini, "Clearing", "iXPClearAltar", GetSettingInt("iXPClearAltar"));
	SetIntValue(ini, "Clearing", "iXPClearRock", GetSettingInt("iXPClearRock"));
	SetIntValue(ini, "Clearing", "iXPClearLighthouse", GetSettingInt("iXPClearLighthouse"));
	SetIntValue(ini, "Clearing", "iXPClearOrcStronghold", GetSettingInt("iXPClearOrcStronghold"));
	SetIntValue(ini, "Clearing", "iXPClearGiantCamp", GetSettingInt("iXPClearGiantCamp"));
	SetIntValue(ini, "Clearing", "iXPClearShack", GetSettingInt("iXPClearShack"));
	SetIntValue(ini, "Clearing", "iXPClearNordicTower", GetSettingInt("iXPClearNordicTower"));
	SetIntValue(ini, "Clearing", "iXPClearNordicDwelling", GetSettingInt("iXPClearNordicDwelling"));
	SetIntValue(ini, "Clearing", "iXPClearDocks", GetSettingInt("iXPClearDocks"));
	SetIntValue(ini, "Clearing", "iXPClearDaedricShrine", GetSettingInt("iXPClearDaedricShrine"));
	SetIntValue(ini, "Clearing", "iXPClearCastle", GetSettingInt("iXPClearCastle"));
	//SetIntValue(ini, "Clearing", "iXPClearCapitol", GetSettingInt("iXPClearCapitol"));
	//SetIntValue(ini, "Clearing", "iXPClearMiraakTemple", GetSettingInt("iXPClearMiraakTemple"));
	//SetIntValue(ini, "Clearing", "iXPClearRavenRock", GetSettingInt("iXPClearRavenRock"));
	//SetIntValue(ini, "Clearing", "iXPClearStandingStone", GetSettingInt("iXPClearStandingStone"));
	//SetIntValue(ini, "Clearing", "iXPClearTelvanniTower", GetSettingInt("iXPClearTelvanniTower"));
	//SetIntValue(ini, "Clearing", "iXPClearCastleKarstaag", GetSettingInt("iXPClearCastleKarstaag"));
	SetIntValue(ini, "Clearing", "iXPClearDefault", GetSettingInt("iXPClearDefault"));

	// killing
	SetIntValue(ini, "Killing", "iXPLevelRange", GetSettingInt("iXPLevelRange"));
	SetFloatValue(ini, "Killing", "fXPKillingMult", GetSettingFloat("fXPKillingMult"));
	SetFloatValue(ini, "Killing", "fXPGroupFactor", GetSettingFloat("fXPGroupFactor"));

	// reading
	SetFloatValue(ini, "Reading", "fXPReadingMult", GetSettingFloat("fXPReadingMult"));

	if (ini.SaveFile(path.c_str()) == SI_FILE) {
		logger::warn("Failed to save file: {}", path);
		return;
	}
	logger::info("Settings saved");
}
