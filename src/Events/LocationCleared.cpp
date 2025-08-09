#include "Events/LocationCleared.h"

#include "Settings/Settings.h"
#include "Skyrim/HUDMenu.h"

#include "Hooks/BGSLocation.h"
#include "Utils/GameSettings.h"

using namespace RE;

LocationClearedEventHandler::LocationClearedEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager)
{
	LocationCleared::GetEventSource()->AddEventSink(this);
}

LocationClearedEventHandler::~LocationClearedEventHandler()
{
	LocationCleared::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl LocationClearedEventHandler::ProcessEvent(const LocationCleared::Event*, LocationClearedEventSource*)
{
	BGSLocationEx* location = BGSLocationEx::GetLastChecked();
	if (location) {

		auto type = location->GetMapMarkerType();
		auto name = location->GetName();

		ShowLocationCleared(name);

		logger::info("[LocationCleared] {} (type: {})", 
			name, static_cast<int>(type));

		auto reward = GetReward(type);
		AddExperience(reward);
	}
	return BSEventNotifyControl::kContinue;
}

int LocationClearedEventHandler::GetReward(MarkerType type)
{
	switch (type) {
	case MarkerType::kNone:
		return Settings::Clearing::XPClearDefault;
	case MarkerType::kCity:
		return Settings::Clearing::XPClearCity;
	case MarkerType::kTown:
		return Settings::Clearing::XPClearTown;
	case MarkerType::kSettlement:
		return Settings::Clearing::XPClearSettlement;
	case MarkerType::kCave:
		return Settings::Clearing::XPClearCave;
	case MarkerType::kCamp:
		return Settings::Clearing::XPClearCamp;
	case MarkerType::kFort:
		return Settings::Clearing::XPClearFort;
	case MarkerType::kNordicRuin:
		return Settings::Clearing::XPClearNordicRuin;
	case MarkerType::kDwemerRuin:
		return Settings::Clearing::XPClearDwemerRuin;
	case MarkerType::kShipwreck:
		return Settings::Clearing::XPClearShipwreck;
	case MarkerType::kGrove:
		return Settings::Clearing::XPClearGrove;
	case MarkerType::kLandmark:
		return Settings::Clearing::XPClearLandmark;
	case MarkerType::kDragonLair:
		return Settings::Clearing::XPClearDragonLair;
	case MarkerType::kFarm:
		return Settings::Clearing::XPClearFarm;
	case MarkerType::kWoodMill:
		return Settings::Clearing::XPClearWoodMill;
	case MarkerType::kMine:
		return Settings::Clearing::XPClearMine;
	case MarkerType::kImperialCamp:
	case MarkerType::kStormcloakCamp:
		return Settings::Clearing::XPClearMilitaryCamp;
	case MarkerType::kDoomstone:
		return Settings::Clearing::XPClearDoomstone;
	case MarkerType::kWheatMill:
		return Settings::Clearing::XPClearWheatMill;
	case MarkerType::kSmelter:
		return Settings::Clearing::XPClearSmelter;
	case MarkerType::kStable:
		return Settings::Clearing::XPClearStable;
	case MarkerType::kImperialTower:
		return Settings::Clearing::XPClearImperialTower;
	case MarkerType::kClearing:
		return Settings::Clearing::XPClearClearing;
	case MarkerType::kPass:
		return Settings::Clearing::XPClearPass;
	case MarkerType::kAltar:
		return Settings::Clearing::XPClearAltar;
	case MarkerType::kRock:
		return Settings::Clearing::XPClearRock;
	case MarkerType::kLighthouse:
		return Settings::Clearing::XPClearLighthouse;
	case MarkerType::kOrcStronghold:
		return Settings::Clearing::XPClearOrcStronghold;
	case MarkerType::kGiantCamp:
		return Settings::Clearing::XPClearGiantCamp;
	case MarkerType::kShack:
		return Settings::Clearing::XPClearShack;
	case MarkerType::kNordicTower:
		return Settings::Clearing::XPClearNordicTower;
	case MarkerType::kNordicDwelling:
		return Settings::Clearing::XPClearNordicDwelling;
	case MarkerType::kDocks:
		return Settings::Clearing::XPClearDocks;
	case MarkerType::kShrine:
		return Settings::Clearing::XPClearDaedricShrine;
	case MarkerType::kRiftenCastle:
	case MarkerType::kWindhelmCastle:
	case MarkerType::kWhiterunCastle:
	case MarkerType::kSolitudeCastle:
	case MarkerType::kMarkarthCastle:
	case MarkerType::kWinterholdCastle:
	case MarkerType::kMorthalCastle:
	case MarkerType::kFalkreathCastle:
	case MarkerType::kDawnstarCastle:
		return Settings::Clearing::XPClearCastle;
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
		return Settings::Clearing::XPClearCity;
	case MarkerType::kDLC02MiraakTemple:
		return Settings::Clearing::XPClearMiraakTemple;
	case MarkerType::kDLC02StandingStone:
		return Settings::Clearing::XPClearStandingStone;
	case MarkerType::kDLC02TelvanniTower:
		return Settings::Clearing::XPClearTelvanniTower;
	//case MarkerType::kDLC02ToSkyrim:
	//case MarkerType::kDLC02ToSolstheim:
	case MarkerType::kDLC02CastleKarstaag:
		return Settings::Clearing::XPClearCastleKarstaag;
	default:
		logger::warn("Unknown location type: {}", static_cast<int>(type));
		return Settings::Clearing::XPClearDefault;
	}
}

void LocationClearedEventHandler::ShowLocationCleared(const char* text)
{
	auto status = Utils::GetGameSettingString("sCleared");

	HUDMenuEx::ShowNotification(text, status, "");
}
