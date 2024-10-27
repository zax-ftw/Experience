#include "Events/LocationDiscovery.h"

#include "Settings.h"

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

		logger::info("[LocationDiscovery] {0} ({1})", 
			name, EnumToString(type));

		auto reward = GetReward(type);
		AddExperience(reward);
	}
	return BSEventNotifyControl::kContinue;
}

int LocationDiscoveryEventHandler::GetReward(MarkerType type)
{
	auto settings = Settings::GetSingleton();

	switch (type) {
	case MarkerType::kNone:
		return settings->GetValue<int>("iXPDiscDefault");
	case MarkerType::kCity:
		return settings->GetValue<int>("iXPDiscCity");
	case MarkerType::kTown:
		return settings->GetValue<int>("iXPDiscTown");
	case MarkerType::kSettlement:
		return settings->GetValue<int>("iXPDiscSettlement");
	case MarkerType::kCave:
		return settings->GetValue<int>("iXPDiscCave");
	case MarkerType::kCamp:
		return settings->GetValue<int>("iXPDiscCamp");
	case MarkerType::kFort:
		return settings->GetValue<int>("iXPDiscFort");
	case MarkerType::kNordicRuin:
		return settings->GetValue<int>("iXPDiscNordicRuin");
	case MarkerType::kDwemerRuin:
		return settings->GetValue<int>("iXPDiscDwemerRuin");
	case MarkerType::kShipwreck:
		return settings->GetValue<int>("iXPDiscShipwreck");
	case MarkerType::kGrove:
		return settings->GetValue<int>("iXPDiscGrove");
	case MarkerType::kLandmark:
		return settings->GetValue<int>("iXPDiscLandmark");
	case MarkerType::kDragonLair:
		return settings->GetValue<int>("iXPDiscDragonLair");
	case MarkerType::kFarm:
		return settings->GetValue<int>("iXPDiscFarm");
	case MarkerType::kWoodMill:
		return settings->GetValue<int>("iXPDiscWoodMill");
	case MarkerType::kMine:
		return settings->GetValue<int>("iXPDiscMine");
	case MarkerType::kImperialCamp:
	case MarkerType::kStormcloakCamp:
		return settings->GetValue<int>("iXPDiscMilitaryCamp");
	case MarkerType::kDoomstone:
		return settings->GetValue<int>("iXPDiscDoomstone");
	case MarkerType::kWheatMill:
		return settings->GetValue<int>("iXPDiscWheatMill");
	case MarkerType::kSmelter:
		return settings->GetValue<int>("iXPDiscSmelter");
	case MarkerType::kStable:
		return settings->GetValue<int>("iXPDiscStable");
	case MarkerType::kImperialTower:
		return settings->GetValue<int>("iXPDiscImperialTower");
	case MarkerType::kClearing:
		return settings->GetValue<int>("iXPDiscClearing");
	case MarkerType::kPass:
		return settings->GetValue<int>("iXPDiscPass");
	case MarkerType::kAltar:
		return settings->GetValue<int>("iXPDiscAltar");
	case MarkerType::kRock:
		return settings->GetValue<int>("iXPDiscRock");
	case MarkerType::kLighthouse:
		return settings->GetValue<int>("iXPDiscLighthouse");
	case MarkerType::kOrcStronghold:
		return settings->GetValue<int>("iXPDiscOrcStronghold");
	case MarkerType::kGiantCamp:
		return settings->GetValue<int>("iXPDiscGiantCamp");
	case MarkerType::kShack:
		return settings->GetValue<int>("iXPDiscShack");
	case MarkerType::kNordicTower:
		return settings->GetValue<int>("iXPDiscNordicTower");
	case MarkerType::kNordicDwelling:
		return settings->GetValue<int>("iXPDiscNordicDwelling");
	case MarkerType::kDocks:
		return settings->GetValue<int>("iXPDiscDocks");
	case MarkerType::kShrine:
		return settings->GetValue<int>("iXPDiscDaedricShrine");
	case MarkerType::kRiftenCastle:
	case MarkerType::kWindhelmCastle:
	case MarkerType::kWhiterunCastle:
	case MarkerType::kSolitudeCastle:
	case MarkerType::kMarkarthCastle:
	case MarkerType::kWinterholdCastle:
	case MarkerType::kMorthalCastle:
	case MarkerType::kFalkreathCastle:
	case MarkerType::kDawnstarCastle:
		return settings->GetValue<int>("iXPDiscCastle");
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
		return settings->GetValue<int>("iXPDiscCity");
	case MarkerType::kDLC02MiraakTemple:		
		return settings->GetValue<int>("iXPDiscMiraakTemple");
	case MarkerType::kDLC02StandingStone:
		return settings->GetValue<int>("iXPDiscStandingStone");
	case MarkerType::kDLC02TelvanniTower:	
		return settings->GetValue<int>("iXPDiscTelvanniTower");
	//case MarkerType::kDLC02ToSkyrim:
	//case MarkerType::kDLC02ToSolstheim:
	case MarkerType::kDLC02CastleKarstaag:
		return settings->GetValue<int>("iXPDiscCastleKarstaag");
	default:
		logger::warn("Unknown location type: {}", static_cast<int>(type));
		return settings->GetValue<int>("iXPDiscDefault");
	}
}
