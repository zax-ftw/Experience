#include "Events/LocationCleared.h"

#include "Settings.h"
#include "HUD.h"

#include "Hooks/BGSLocation.h"

using namespace RE;

LocationClearedEventHandler::LocationClearedEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kActive)
{
	LocationCleared::GetEventSource()->AddEventSink(this);
}

LocationClearedEventHandler::~LocationClearedEventHandler(void)
{
	LocationCleared::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl LocationClearedEventHandler::ProcessEvent(const LocationCleared::Event*, LocationClearedEventSource*)
{
	BGSLocationEx* location = BGSLocationEx::GetLastChecked();
	if (location && location->cleared) {

		auto type = location->GetMapMarkerType();
		auto name = location->GetName();

		ShowLocationCleared(name);

		logger::info("LocationCleared: {0} ({1})", 
			name, magic_enum::enum_name(type));

		auto reward = GetReward(type);
		AddExperience(reward);
	}
	return BSEventNotifyControl::kContinue;
}

int LocationClearedEventHandler::GetReward(MarkerType type)
{
	auto settings = Settings::GetSingleton();

	switch (type) {
	case MarkerType::kCity:
		return settings->GetSettingInt("iXPClearCity");
	case MarkerType::kTown:
		return settings->GetSettingInt("iXPClearTown");
	case MarkerType::kSettlement:
		return settings->GetSettingInt("iXPClearSettlement");
	case MarkerType::kCave:
		return settings->GetSettingInt("iXPClearCave");
	case MarkerType::kCamp:
		return settings->GetSettingInt("iXPClearCamp");
	case MarkerType::kFort:
		return settings->GetSettingInt("iXPClearFort");
	case MarkerType::kNordicRuin:
		return settings->GetSettingInt("iXPClearNordicRuin");
	case MarkerType::kDwemerRuin:
		return settings->GetSettingInt("iXPClearDwemerRuin");
	case MarkerType::kShipwreck:
		return settings->GetSettingInt("iXPClearShipwreck");
	case MarkerType::kGrove:
		return settings->GetSettingInt("iXPClearGrove");
	case MarkerType::kLandmark:
		return settings->GetSettingInt("iXPClearLandmark");
	case MarkerType::kDragonLair:
		return settings->GetSettingInt("iXPClearDragonLair");
	case MarkerType::kFarm:
		return settings->GetSettingInt("iXPClearFarm");
	case MarkerType::kWoodMill:
		return settings->GetSettingInt("iXPClearWoodMill");
	case MarkerType::kMine:
		return settings->GetSettingInt("iXPClearMine");
	case MarkerType::kImperialCamp:
	case MarkerType::kStormcloakCamp:
		return settings->GetSettingInt("iXPClearMilitaryCamp");
	case MarkerType::kDoomstone:
		return settings->GetSettingInt("iXPClearDoomstone");
	case MarkerType::kWheatMill:
		return settings->GetSettingInt("iXPClearWheatMill");
	case MarkerType::kSmelter:
		return settings->GetSettingInt("iXPClearSmelter");
	case MarkerType::kStable:
		return settings->GetSettingInt("iXPClearStable");
	case MarkerType::kImperialTower:
		return settings->GetSettingInt("iXPClearImperialTower");
	case MarkerType::kClearing:
		return settings->GetSettingInt("iXPClearClearing");
	case MarkerType::kPass:
		return settings->GetSettingInt("iXPClearPass");
	case MarkerType::kAltar:
		return settings->GetSettingInt("iXPClearAltar");
	case MarkerType::kRock:
		return settings->GetSettingInt("iXPClearRock");
	case MarkerType::kLighthouse:
		return settings->GetSettingInt("iXPClearLighthouse");
	case MarkerType::kOrcStronghold:
		return settings->GetSettingInt("iXPClearOrcStronghold");
	case MarkerType::kGiantCamp:
		return settings->GetSettingInt("iXPClearGiantCamp");
	case MarkerType::kShack:
		return settings->GetSettingInt("iXPClearShack");
	case MarkerType::kNordicTower:
		return settings->GetSettingInt("iXPClearNordicTower");
	case MarkerType::kNordicDwelling:
		return settings->GetSettingInt("iXPClearNordicDwelling");
	case MarkerType::kDocks:
		return settings->GetSettingInt("iXPClearDocks");
	case MarkerType::kShrine:
		return settings->GetSettingInt("iXPClearDaedricShrine");
	case MarkerType::kRiftenCastle:
	case MarkerType::kWindhelmCastle:
	case MarkerType::kWhiterunCastle:
	case MarkerType::kSolitudeCastle:
	case MarkerType::kMarkarthCastle:
	case MarkerType::kWinterholdCastle:
	case MarkerType::kMorthalCastle:
	case MarkerType::kFalkreathCastle:
	case MarkerType::kDawnstarCastle:
		return settings->GetSettingInt("iXPClearCastle");
	case MarkerType::kRiftenCapitol:
	case MarkerType::kWindhelmCapitol:
	case MarkerType::kWhiterunCapitol:
	case MarkerType::kSolitudeCapitol:
	case MarkerType::kMarkarthCapitol:
	case MarkerType::kWinterholdCapitol:
	case MarkerType::kMorthalCapitol:
	case MarkerType::kFalkreathCapitol:
	case MarkerType::kDawnstarCapitol:
	case MarkerType::kDLC02RavenRock:
		return settings->GetSettingInt("iXPClearCity");
	case MarkerType::kDLC02MiraakTemple:
		return settings->GetSettingInt("iXPClearMiraakTemple");
	case MarkerType::kDLC02StandingStone:
		return settings->GetSettingInt("iXPClearStandingStone");
	case MarkerType::kDLC02TelvanniTower:
		return settings->GetSettingInt("iXPClearTelvanniTower");
	//case MarkerType::kDLC02ToSkyrim:
	//case MarkerType::kDLC02ToSolstheim:	
	case MarkerType::kDLC02CastleKarstaag:
		return settings->GetSettingInt("iXPClearCastleKarstaag");
	default:
		return settings->GetSettingInt("iXPClearDefault");
	}
}

void LocationClearedEventHandler::ShowLocationCleared(const char* text)
{
	using Type = HUD::MessageType;

	auto settings = GameSettingCollection::GetSingleton();
	Setting* setting = settings->GetSetting("sCleared");
	if (setting) {
		const char* status = setting->GetString();
		HUD::ShowNotification(text, status, "", Type::kQuestStarted);
	}
}
