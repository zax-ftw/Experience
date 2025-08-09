#pragma once

#include "SettingField.h"
#include "Utils/Singleton.h"

#define SETTING_INT(NAME, SECTION, DEFAULT, ...) \
	inline static SettingField<int> NAME { SECTION, "i" #NAME, DEFAULT __VA_OPT__(, ) __VA_ARGS__ }

#define SETTING_BOOL(NAME, SECTION, DEFAULT, ...) \
	inline static SettingField<bool> NAME { SECTION, "b" #NAME, DEFAULT __VA_OPT__(, ) __VA_ARGS__ }

#define SETTING_FLOAT(NAME, SECTION, DEFAULT, ...) \
	inline static SettingField<float> NAME { SECTION, "f" #NAME, DEFAULT __VA_OPT__(, ) __VA_ARGS__ }

#define SETTING_STRING(NAME, SECTION, DEFAULT, ...) \
	inline static SettingField<std::string> NAME { SECTION, "s" #NAME, DEFAULT __VA_OPT__(, ) __VA_ARGS__ }

class Settings : public ISingleton<Settings>
{
public:
	friend class ISingleton<Settings>;

	struct General
	{
		SETTING_BOOL(EnableKilling, "General", true);
		SETTING_BOOL(EnableReading, "General", true);
		SETTING_BOOL(EnableSkillXP, "General", false);
		SETTING_INT(MaxPlayerLevel, "General", 255);
		SETTING_FLOAT(SkillCapBase, "General", 18.0f);
		SETTING_FLOAT(SkillCapMult, "General", 2.0f);
		SETTING_BOOL(UseRacialCaps, "General", true);
		SETTING_BOOL(ShowMessages, "General", true);
		SETTING_STRING(MessageFormat, "General", "{:+d} XP");
		SETTING_INT(MeterMode, "General", 1);
	};

	struct Questing
	{
		SETTING_INT(XPQuestObjectives, "Questing", 10);
		SETTING_INT(XPQuestNone, "Questing", 0);
		SETTING_INT(XPQuestMain, "Questing", 75);
		SETTING_INT(XPQuestCollege, "Questing", 50);
		SETTING_INT(XPQuestThieves, "Questing", 50);
		SETTING_INT(XPQuestBrotherhood, "Questing", 50);
		SETTING_INT(XPQuestCompanions, "Questing", 50);
		SETTING_INT(XPQuestMisc, "Questing", 0);
		SETTING_INT(XPQuestDaedric, "Questing", 75);
		SETTING_INT(XPQuestSide, "Questing", 50);
		SETTING_INT(XPQuestCivilWar, "Questing", 75);
		SETTING_INT(XPQuestDawnguard, "Questing", 50);
		SETTING_INT(XPQuestDragonborn, "Questing", 75);
	};

	struct Exploring
	{
		SETTING_INT(XPDiscDefault, "Exploring", 10);
		SETTING_INT(XPDiscCity, "Exploring", 10);
		SETTING_INT(XPDiscTown, "Exploring", 10);
		SETTING_INT(XPDiscSettlement, "Exploring", 10);
		SETTING_INT(XPDiscCave, "Exploring", 10);
		SETTING_INT(XPDiscCamp, "Exploring", 10);
		SETTING_INT(XPDiscFort, "Exploring", 10);
		SETTING_INT(XPDiscNordicRuin, "Exploring", 10);
		SETTING_INT(XPDiscDwemerRuin, "Exploring", 10);
		SETTING_INT(XPDiscShipwreck, "Exploring", 10);
		SETTING_INT(XPDiscGrove, "Exploring", 10);
		SETTING_INT(XPDiscLandmark, "Exploring", 10);
		SETTING_INT(XPDiscDragonLair, "Exploring", 10);
		SETTING_INT(XPDiscFarm, "Exploring", 10);
		SETTING_INT(XPDiscWoodMill, "Exploring", 10);
		SETTING_INT(XPDiscMine, "Exploring", 10);
		SETTING_INT(XPDiscMilitaryCamp, "Exploring", 10);
		SETTING_INT(XPDiscDoomstone, "Exploring", 10);
		SETTING_INT(XPDiscWheatMill, "Exploring", 10);
		SETTING_INT(XPDiscSmelter, "Exploring", 10);
		SETTING_INT(XPDiscStable, "Exploring", 10);
		SETTING_INT(XPDiscImperialTower, "Exploring", 10);
		SETTING_INT(XPDiscClearing, "Exploring", 10);
		SETTING_INT(XPDiscPass, "Exploring", 10);
		SETTING_INT(XPDiscAltar, "Exploring", 10);
		SETTING_INT(XPDiscRock, "Exploring", 10);
		SETTING_INT(XPDiscLighthouse, "Exploring", 10);
		SETTING_INT(XPDiscOrcStronghold, "Exploring", 10);
		SETTING_INT(XPDiscGiantCamp, "Exploring", 10);
		SETTING_INT(XPDiscShack, "Exploring", 10);
		SETTING_INT(XPDiscNordicTower, "Exploring", 10);
		SETTING_INT(XPDiscNordicDwelling, "Exploring", 10);
		SETTING_INT(XPDiscDocks, "Exploring", 10);
		SETTING_INT(XPDiscDaedricShrine, "Exploring", 10);
		SETTING_INT(XPDiscCastle, "Exploring", 10);
		SETTING_INT(XPDiscMiraakTemple, "Exploring", 10);
		SETTING_INT(XPDiscStandingStone, "Exploring", 10);
		SETTING_INT(XPDiscTelvanniTower, "Exploring", 10);
		SETTING_INT(XPDiscCastleKarstaag, "Exploring", 10);
	};

	struct Clearing
	{
		SETTING_INT(XPClearDefault, "Clearing", 30);
		SETTING_INT(XPClearCity, "Clearing", 30);
		SETTING_INT(XPClearTown, "Clearing", 30);
		SETTING_INT(XPClearSettlement, "Clearing", 30);
		SETTING_INT(XPClearCave, "Clearing", 30);
		SETTING_INT(XPClearCamp, "Clearing", 30);
		SETTING_INT(XPClearFort, "Clearing", 30);
		SETTING_INT(XPClearNordicRuin, "Clearing", 30);
		SETTING_INT(XPClearDwemerRuin, "Clearing", 30);
		SETTING_INT(XPClearShipwreck, "Clearing", 30);
		SETTING_INT(XPClearGrove, "Clearing", 30);
		SETTING_INT(XPClearLandmark, "Clearing", 30);
		SETTING_INT(XPClearDragonLair, "Clearing", 30);
		SETTING_INT(XPClearFarm, "Clearing", 30);
		SETTING_INT(XPClearWoodMill, "Clearing", 30);
		SETTING_INT(XPClearMine, "Clearing", 30);
		SETTING_INT(XPClearMilitaryCamp, "Clearing", 30);
		SETTING_INT(XPClearDoomstone, "Clearing", 30);
		SETTING_INT(XPClearWheatMill, "Clearing", 30);
		SETTING_INT(XPClearSmelter, "Clearing", 30);
		SETTING_INT(XPClearStable, "Clearing", 30);
		SETTING_INT(XPClearImperialTower, "Clearing", 30);
		SETTING_INT(XPClearClearing, "Clearing", 30);
		SETTING_INT(XPClearPass, "Clearing", 30);
		SETTING_INT(XPClearAltar, "Clearing", 30);
		SETTING_INT(XPClearRock, "Clearing", 30);
		SETTING_INT(XPClearLighthouse, "Clearing", 30);
		SETTING_INT(XPClearOrcStronghold, "Clearing", 30);
		SETTING_INT(XPClearGiantCamp, "Clearing", 30);
		SETTING_INT(XPClearShack, "Clearing", 30);
		SETTING_INT(XPClearNordicTower, "Clearing", 30);
		SETTING_INT(XPClearNordicDwelling, "Clearing", 30);
		SETTING_INT(XPClearDocks, "Clearing", 30);
		SETTING_INT(XPClearDaedricShrine, "Clearing", 30);
		SETTING_INT(XPClearCastle, "Clearing", 30);
		SETTING_INT(XPClearMiraakTemple, "Clearing", 30);
		SETTING_INT(XPClearStandingStone, "Clearing", 30);
		SETTING_INT(XPClearTelvanniTower, "Clearing", 30);
		SETTING_INT(XPClearCastleKarstaag, "Clearing", 30);
	};

	struct Killing
	{
		SETTING_FLOAT(LevelFactor, "Killing", 0.8f);
		SETTING_FLOAT(GroupFactor, "Killing", 0.0f);
		SETTING_FLOAT(DamageThreshold, "Killing", 0.2f);
		SETTING_FLOAT(KillingMult, "Killing", 1.0f);
	};

	struct Reading
	{
		SETTING_FLOAT(ReadingMult, "Reading", 2.0f);
	};

	void ReadSettings();
	void WriteSettings();
	void DumpSettings() const;

	template <typename T>
	T GetValue(std::string_view key) const;

	template <typename T>
	void SetValue(std::string_view key, T value);

	void Register(ISettingField* field);

private:

	std::vector<ISettingField*> fields;
	mutable std::shared_mutex mtx;

	static constexpr const char* path = "Data/SKSE/Plugins/Experience.ini";
};
