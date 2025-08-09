#include "Events/LocationDiscovery.h"

#include "Settings/Settings.h"

using namespace RE;

LocationDiscoveryEventHandler::LocationDiscoveryEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager)
{
	LocationDiscovery::GetEventSource()->AddEventSink(this);
}

LocationDiscoveryEventHandler::~LocationDiscoveryEventHandler()
{
	LocationDiscovery::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl LocationDiscoveryEventHandler::ProcessEvent(const LocationDiscovery::Event* event, LocationDiscoveryEventSource*)
{
	MapMarkerData* data = event->mapMarkerData;
	if (data) {

		auto name = data->locationName.GetFullName();
		auto type = data->type.get();

		logger::info("[LocationDiscovery] {} (type: {})", 
			name, static_cast<int>(type));

		auto reward = GetReward(type);
		AddExperience(reward);
	}
	return BSEventNotifyControl::kContinue;
}

int LocationDiscoveryEventHandler::GetReward(MarkerType type)
{
	switch (type) {
	case MarkerType::kNone:
		return Settings::Exploring::XPDiscDefault;
	case MarkerType::kCity:
		return Settings::Exploring::XPDiscCity;
	case MarkerType::kTown:
		return Settings::Exploring::XPDiscTown;
	case MarkerType::kSettlement:
		return Settings::Exploring::XPDiscSettlement;
	case MarkerType::kCave:
		return Settings::Exploring::XPDiscCave;
	case MarkerType::kCamp:
		return Settings::Exploring::XPDiscCamp;
	case MarkerType::kFort:
		return Settings::Exploring::XPDiscFort;
	case MarkerType::kNordicRuin:
		return Settings::Exploring::XPDiscNordicRuin;
	case MarkerType::kDwemerRuin:
		return Settings::Exploring::XPDiscDwemerRuin;
	case MarkerType::kShipwreck:
		return Settings::Exploring::XPDiscShipwreck;
	case MarkerType::kGrove:
		return Settings::Exploring::XPDiscGrove;
	case MarkerType::kLandmark:
		return Settings::Exploring::XPDiscLandmark;
	case MarkerType::kDragonLair:
		return Settings::Exploring::XPDiscDragonLair;
	case MarkerType::kFarm:
		return Settings::Exploring::XPDiscFarm;
	case MarkerType::kWoodMill:
		return Settings::Exploring::XPDiscWoodMill;
	case MarkerType::kMine:
		return Settings::Exploring::XPDiscMine;
	case MarkerType::kImperialCamp:
	case MarkerType::kStormcloakCamp:
		return Settings::Exploring::XPDiscMilitaryCamp;
	case MarkerType::kDoomstone:
		return Settings::Exploring::XPDiscDoomstone;
	case MarkerType::kWheatMill:
		return Settings::Exploring::XPDiscWheatMill;
	case MarkerType::kSmelter:
		return Settings::Exploring::XPDiscSmelter;
	case MarkerType::kStable:
		return Settings::Exploring::XPDiscStable;
	case MarkerType::kImperialTower:
		return Settings::Exploring::XPDiscImperialTower;
	case MarkerType::kClearing:
		return Settings::Exploring::XPDiscClearing;
	case MarkerType::kPass:
		return Settings::Exploring::XPDiscPass;
	case MarkerType::kAltar:
		return Settings::Exploring::XPDiscAltar;
	case MarkerType::kRock:
		return Settings::Exploring::XPDiscRock;
	case MarkerType::kLighthouse:
		return Settings::Exploring::XPDiscLighthouse;
	case MarkerType::kOrcStronghold:
		return Settings::Exploring::XPDiscOrcStronghold;
	case MarkerType::kGiantCamp:
		return Settings::Exploring::XPDiscGiantCamp;
	case MarkerType::kShack:
		return Settings::Exploring::XPDiscShack;
	case MarkerType::kNordicTower:
		return Settings::Exploring::XPDiscNordicTower;
	case MarkerType::kNordicDwelling:
		return Settings::Exploring::XPDiscNordicDwelling;
	case MarkerType::kDocks:
		return Settings::Exploring::XPDiscDocks;
	case MarkerType::kShrine:
		return Settings::Exploring::XPDiscDaedricShrine;
	case MarkerType::kRiftenCastle:
	case MarkerType::kWindhelmCastle:
	case MarkerType::kWhiterunCastle:
	case MarkerType::kSolitudeCastle:
	case MarkerType::kMarkarthCastle:
	case MarkerType::kWinterholdCastle:
	case MarkerType::kMorthalCastle:
	case MarkerType::kFalkreathCastle:
	case MarkerType::kDawnstarCastle:
		return Settings::Exploring::XPDiscCastle;
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
		return Settings::Exploring::XPDiscCity;
	case MarkerType::kDLC02MiraakTemple:
		return Settings::Exploring::XPDiscMiraakTemple;
	case MarkerType::kDLC02StandingStone:
		return Settings::Exploring::XPDiscStandingStone;
	case MarkerType::kDLC02TelvanniTower:
		return Settings::Exploring::XPDiscTelvanniTower;
	//case MarkerType::kDLC02ToSkyrim:
	//case MarkerType::kDLC02ToSolstheim:
	case MarkerType::kDLC02CastleKarstaag:
		return Settings::Exploring::XPDiscCastleKarstaag;
	default:
		logger::warn("Unknown location type: {}", static_cast<int>(type));
		return Settings::Exploring::XPDiscDefault;
	}
}
