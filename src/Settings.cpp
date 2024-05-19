#include "Settings.h"


void Settings::ReadSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	if (ini.LoadFile(path) == SI_FILE) {
		logger::warn("Failed to load file: {}", path);
	}

	ReadBoolSetting(ini, "General", "bEnableKilling", true);
	ReadBoolSetting(ini, "General", "bEnableReading", true);
	ReadBoolSetting(ini, "General", "bEnableSkillXP", false);
	ReadIntSetting(ini, "General", "iMaxPlayerLevel", 255);
	ReadFloatSetting(ini, "General", "fSkillCapBase", 18.0f);
	ReadFloatSetting(ini, "General", "fSkillCapMult", 2.0f);
	ReadBoolSetting(ini, "General", "bUseRacialCaps", true);
	ReadBoolSetting(ini, "General", "bShowMessages", true);
	ReadStringSetting(ini, "General", "sMessageFormat","{:+d} XP");

	// 3.3.0
	int forceLvlMeter = ReadBoolSetting(ini, "General", "bForceLvlMeter", false);
	ReadIntSetting(ini, "General", "iMeterMode", forceLvlMeter + 1);

	ReadIntSetting(ini, "Questing", "iXPQuestNone", 0);
	ReadIntSetting(ini, "Questing", "iXPQuestMain", 75);
	ReadIntSetting(ini, "Questing", "iXPQuestCollege", 50);
	ReadIntSetting(ini, "Questing", "iXPQuestThieves", 50);
	ReadIntSetting(ini, "Questing", "iXPQuestBrotherhood", 50);
	ReadIntSetting(ini, "Questing", "iXPQuestCompanions", 50);
	ReadIntSetting(ini, "Questing", "iXPQuestMisc", 0);
	ReadIntSetting(ini, "Questing", "iXPQuestDaedric", 75);
	ReadIntSetting(ini, "Questing", "iXPQuestSide", 50);
	ReadIntSetting(ini, "Questing", "iXPQuestCivilWar", 75);
	ReadIntSetting(ini, "Questing", "iXPQuestDawnguard", 50);
	ReadIntSetting(ini, "Questing", "iXPQuestDragonborn", 75);
	ReadIntSetting(ini, "Questing", "iXPObjectives", 10);

	int discDefault = ReadIntSetting(ini, "Exploring", "iXPDiscDefault", 10);

	ReadIntSetting(ini, "Exploring", "iXPDiscCity", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscTown", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscSettlement", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscCave", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscCamp", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscFort", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscNordicRuin", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscDwemerRuin", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscShipwreck", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscGrove", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscLandmark", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscDragonLair", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscFarm", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscWoodMill", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscMine", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscMilitaryCamp", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscDoomstone", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscWheatMill", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscSmelter", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscStable", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscImperialTower", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscClearing", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscPass", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscAltar", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscRock", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscLighthouse", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscOrcStronghold", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscGiantCamp", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscShack", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscNordicTower", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscNordicDwelling", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscDocks", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscDaedricShrine", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscCastle", discDefault);
	//ReadIntSetting(ini, "Exploring", "iXPDiscCapitol", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscMiraakTemple", discDefault);
	//ReadIntSetting(ini, "Exploring", "iXPDiscRavenRock", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscStandingStone", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscTelvanniTower", discDefault);
	ReadIntSetting(ini, "Exploring", "iXPDiscCastleKarstaag", discDefault);

	int clearDefault = ReadIntSetting(ini, "Clearing", "iXPClearDefault", 30);

	ReadIntSetting(ini, "Clearing", "iXPClearCity", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearTown", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearSettlement", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearCave", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearCamp", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearFort", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearNordicRuin", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearDwemerRuin", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearShipwreck", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearGrove", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearLandmark", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearDragonLair", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearFarm", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearWoodMill", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearMine", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearMilitaryCamp", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearDoomstone", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearWheatMill", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearSmelter", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearStable", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearImperialTower", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearClearing", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearPass", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearAltar", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearRock", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearLighthouse", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearOrcStronghold", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearGiantCamp", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearShack", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearNordicTower", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearNordicDwelling", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearDocks", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearDaedricShrine", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearCastle", clearDefault);
	//ReadIntSetting(ini, "Clearing", "iXPClearCapitol", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearMiraakTemple", clearDefault);
	//ReadIntSetting(ini, "Clearing", "iXPClearRavenRock", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearStandingStone", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearTelvanniTower", clearDefault);
	ReadIntSetting(ini, "Clearing", "iXPClearCastleKarstaag", clearDefault);

	ReadFloatSetting(ini, "Killing", "fXPLevelFactor", 0.8f);
	ReadFloatSetting(ini, "Killing", "fXPKillingMult", 1.0f);
	ReadFloatSetting(ini, "Killing", "fXPGroupFactor", 0.2f);

	ReadFloatSetting(ini, "Reading", "fXPReadingMult", 2.0f);
}

void Settings::WriteSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	WriteBoolSetting(ini, "General", "bEnableKilling");
	WriteBoolSetting(ini, "General", "bEnableReading");
	WriteBoolSetting(ini, "General", "bEnableSkillXP");
	WriteIntSetting(ini, "General", "iMaxPlayerLevel");
	WriteFloatSetting(ini, "General", "fSkillCapBase");
	WriteFloatSetting(ini, "General", "fSkillCapMult");
	WriteBoolSetting(ini, "General", "bUseRacialCaps");
	WriteBoolSetting(ini, "General", "bShowMessages");
	WriteStringSetting(ini, "General", "sMessageFormat");
	WriteIntSetting(ini, "General", "iMeterMode");

	WriteIntSetting(ini, "Questing", "iXPQuestNone");
	WriteIntSetting(ini, "Questing", "iXPQuestMain");
	WriteIntSetting(ini, "Questing", "iXPQuestCollege");
	WriteIntSetting(ini, "Questing", "iXPQuestThieves");
	WriteIntSetting(ini, "Questing", "iXPQuestBrotherhood");
	WriteIntSetting(ini, "Questing", "iXPQuestCompanions");
	WriteIntSetting(ini, "Questing", "iXPQuestMisc");
	WriteIntSetting(ini, "Questing", "iXPQuestDaedric");
	WriteIntSetting(ini, "Questing", "iXPQuestSide");
	WriteIntSetting(ini, "Questing", "iXPQuestCivilWar");
	WriteIntSetting(ini, "Questing", "iXPQuestDawnguard");
	WriteIntSetting(ini, "Questing", "iXPQuestDragonborn");
	WriteIntSetting(ini, "Questing", "iXPObjectives");

	WriteIntSetting(ini, "Exploring", "iXPDiscCity");
	WriteIntSetting(ini, "Exploring", "iXPDiscTown");
	WriteIntSetting(ini, "Exploring", "iXPDiscSettlement");
	WriteIntSetting(ini, "Exploring", "iXPDiscCave");
	WriteIntSetting(ini, "Exploring", "iXPDiscCamp");
	WriteIntSetting(ini, "Exploring", "iXPDiscFort");
	WriteIntSetting(ini, "Exploring", "iXPDiscNordicRuin");
	WriteIntSetting(ini, "Exploring", "iXPDiscDwemerRuin");
	WriteIntSetting(ini, "Exploring", "iXPDiscShipwreck");
	WriteIntSetting(ini, "Exploring", "iXPDiscGrove");
	WriteIntSetting(ini, "Exploring", "iXPDiscLandmark");
	WriteIntSetting(ini, "Exploring", "iXPDiscDragonLair");
	WriteIntSetting(ini, "Exploring", "iXPDiscFarm");
	WriteIntSetting(ini, "Exploring", "iXPDiscWoodMill");
	WriteIntSetting(ini, "Exploring", "iXPDiscMine");
	WriteIntSetting(ini, "Exploring", "iXPDiscMilitaryCamp");
	WriteIntSetting(ini, "Exploring", "iXPDiscDoomstone");
	WriteIntSetting(ini, "Exploring", "iXPDiscWheatMill");
	WriteIntSetting(ini, "Exploring", "iXPDiscSmelter");
	WriteIntSetting(ini, "Exploring", "iXPDiscStable");
	WriteIntSetting(ini, "Exploring", "iXPDiscImperialTower");
	WriteIntSetting(ini, "Exploring", "iXPDiscClearing");
	WriteIntSetting(ini, "Exploring", "iXPDiscPass");
	WriteIntSetting(ini, "Exploring", "iXPDiscAltar");
	WriteIntSetting(ini, "Exploring", "iXPDiscRock");
	WriteIntSetting(ini, "Exploring", "iXPDiscLighthouse");
	WriteIntSetting(ini, "Exploring", "iXPDiscOrcStronghold");
	WriteIntSetting(ini, "Exploring", "iXPDiscGiantCamp");
	WriteIntSetting(ini, "Exploring", "iXPDiscShack");
	WriteIntSetting(ini, "Exploring", "iXPDiscNordicTower");
	WriteIntSetting(ini, "Exploring", "iXPDiscNordicDwelling");
	WriteIntSetting(ini, "Exploring", "iXPDiscDocks");
	WriteIntSetting(ini, "Exploring", "iXPDiscDaedricShrine");
	WriteIntSetting(ini, "Exploring", "iXPDiscCastle");
	//WriteIntSetting(ini, "Exploring", "iXPDiscCapitol");
	WriteIntSetting(ini, "Exploring", "iXPDiscMiraakTemple");
	//WriteIntSetting(ini, "Exploring", "iXPDiscRavenRock");
	WriteIntSetting(ini, "Exploring", "iXPDiscStandingStone");
	WriteIntSetting(ini, "Exploring", "iXPDiscTelvanniTower");
	WriteIntSetting(ini, "Exploring", "iXPDiscCastleKarstaag");
	WriteIntSetting(ini, "Exploring", "iXPDiscDefault");

	WriteIntSetting(ini, "Clearing", "iXPClearCity");
	WriteIntSetting(ini, "Clearing", "iXPClearTown");
	WriteIntSetting(ini, "Clearing", "iXPClearSettlement");
	WriteIntSetting(ini, "Clearing", "iXPClearCave");
	WriteIntSetting(ini, "Clearing", "iXPClearCamp");
	WriteIntSetting(ini, "Clearing", "iXPClearFort");
	WriteIntSetting(ini, "Clearing", "iXPClearNordicRuin");
	WriteIntSetting(ini, "Clearing", "iXPClearDwemerRuin");
	WriteIntSetting(ini, "Clearing", "iXPClearShipwreck");
	WriteIntSetting(ini, "Clearing", "iXPClearGrove");
	WriteIntSetting(ini, "Clearing", "iXPClearLandmark");
	WriteIntSetting(ini, "Clearing", "iXPClearDragonLair");
	WriteIntSetting(ini, "Clearing", "iXPClearFarm");
	WriteIntSetting(ini, "Clearing", "iXPClearWoodMill");
	WriteIntSetting(ini, "Clearing", "iXPClearMine");
	WriteIntSetting(ini, "Clearing", "iXPClearMilitaryCamp");
	WriteIntSetting(ini, "Clearing", "iXPClearDoomstone");
	WriteIntSetting(ini, "Clearing", "iXPClearWheatMill");
	WriteIntSetting(ini, "Clearing", "iXPClearSmelter");
	WriteIntSetting(ini, "Clearing", "iXPClearStable");
	WriteIntSetting(ini, "Clearing", "iXPClearImperialTower");
	WriteIntSetting(ini, "Clearing", "iXPClearClearing");
	WriteIntSetting(ini, "Clearing", "iXPClearPass");
	WriteIntSetting(ini, "Clearing", "iXPClearAltar");
	WriteIntSetting(ini, "Clearing", "iXPClearRock");
	WriteIntSetting(ini, "Clearing", "iXPClearLighthouse");
	WriteIntSetting(ini, "Clearing", "iXPClearOrcStronghold");
	WriteIntSetting(ini, "Clearing", "iXPClearGiantCamp");
	WriteIntSetting(ini, "Clearing", "iXPClearShack");
	WriteIntSetting(ini, "Clearing", "iXPClearNordicTower");
	WriteIntSetting(ini, "Clearing", "iXPClearNordicDwelling");
	WriteIntSetting(ini, "Clearing", "iXPClearDocks");
	WriteIntSetting(ini, "Clearing", "iXPClearDaedricShrine");
	WriteIntSetting(ini, "Clearing", "iXPClearCastle");
	//WriteIntSetting(ini, "Clearing", "iXPClearCapitol");
	WriteIntSetting(ini, "Clearing", "iXPClearMiraakTemple");
	//WriteIntSetting(ini, "Clearing", "iXPClearRavenRock");
	WriteIntSetting(ini, "Clearing", "iXPClearStandingStone");
	WriteIntSetting(ini, "Clearing", "iXPClearTelvanniTower");
	WriteIntSetting(ini, "Clearing", "iXPClearCastleKarstaag");
	WriteIntSetting(ini, "Clearing", "iXPClearDefault");

	WriteFloatSetting(ini, "Killing", "fXPLevelFactor");
	WriteFloatSetting(ini, "Killing", "fXPGroupFactor");
	WriteFloatSetting(ini, "Killing", "fXPKillingMult");

	WriteFloatSetting(ini, "Reading", "fXPReadingMult");

	if (ini.SaveFile(path) == SI_FILE) {
		logger::warn("Failed to save file: {}", path);
	}
}

Settings::Setting Settings::GetSetting(std::string_view key) const
{
	std::shared_lock lock(mtx);
	auto iter = data.find(key);
	return iter == data.end() ? Setting() : iter->second;
};

int Settings::ReadIntSetting(CSimpleIniA& ini, const char* section, const char* key, int def)
{
	int value = ini.GetLongValue(section, key, def);
	SetValue(key, value);
	return value;
}

float Settings::ReadFloatSetting(CSimpleIniA& ini, const char* section, const char* key, float def)
{
	float value = ini.GetDoubleValue(section, key, def);
	SetValue(key, value);
	return value;
}
bool Settings::ReadBoolSetting(CSimpleIniA& ini, const char* section, const char* key, bool def)
{
	bool value = ini.GetBoolValue(section, key, def);
	SetValue(key, value);
	return value;
}

const char* Settings::ReadStringSetting(CSimpleIniA& ini, const char* section, const char* key, const char* def)
{
	const char* value = ini.GetValue(section, key, def);
	SetValue(key, std::string(value));
	return value;
}

void Settings::WriteIntSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment)
{
	int value = GetValue<int>(key);
	ini.SetLongValue(section, key, value);
}

void Settings::WriteFloatSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment)
{
	float value = GetValue<float>(key);
	ini.SetDoubleValue(section, key, value);
}

void Settings::WriteBoolSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment)
{
	bool value = GetValue<bool>(key);
	ini.SetBoolValue(section, key, value);
}

void Settings::WriteStringSetting(CSimpleIniA& ini, const char* section, const char* key, const char* comment)
{
	std::string value = GetValue<std::string>(key);
	ini.SetValue(section, key, value.c_str());
}
