#include "Events/LocationDiscovery.h"

#include "Settings.h"

using namespace RE;

LocationDiscoveryEventHandler::LocationDiscoveryEventHandler(ExperienceManager* manager) :
	ExperienceManager::Source(manager, MeterState::kActive)
{
	LocationDiscovery::GetEventSource()->AddEventSink(this);
}

LocationDiscoveryEventHandler::~LocationDiscoveryEventHandler(void)
{
	LocationDiscovery::GetEventSource()->RemoveEventSink(this);
}

BSEventNotifyControl LocationDiscoveryEventHandler::ProcessEvent(const LocationDiscovery::Event* event, LocationDiscoveryEventSource*)
{
	MapMarkerData* data = event->mapMarkerData;
	if (data) {

		auto name = data->locationName.GetFullName();
		auto type = data->type.get();

		logger::info("LocationDiscovery: {0} ({1})", 
			name, magic_enum::enum_name(type));

		auto reward = GetReward(type);
		AddExperience(reward);
	}
	return BSEventNotifyControl::kContinue;
}

int LocationDiscoveryEventHandler::GetReward(MarkerType type)
{
	auto settings = Settings::GetSingleton();

	switch (type) {
	case MarkerType::kCity:
		return settings->GetSettingInt("iXPDiscCity");
	case MarkerType::kTown:
		return settings->GetSettingInt("iXPDiscTown");
	case MarkerType::kSettlement:
		return settings->GetSettingInt("iXPDiscSettlement");
	case MarkerType::kCave:
		return settings->GetSettingInt("iXPDiscCave");
	case MarkerType::kCamp:
		return settings->GetSettingInt("iXPDiscCamp");
	case MarkerType::kFort:
		return settings->GetSettingInt("iXPDiscFort");
	case MarkerType::kNordicRuin:
		return settings->GetSettingInt("iXPDiscNordicRuin");
	case MarkerType::kDwemerRuin:
		return settings->GetSettingInt("iXPDiscDwemerRuin");
	case MarkerType::kShipwreck:
		return settings->GetSettingInt("iXPDiscShipwreck");
	case MarkerType::kGrove:
		return settings->GetSettingInt("iXPDiscGrove");
	case MarkerType::kLandmark:
		return settings->GetSettingInt("iXPDiscLandmark");
	case MarkerType::kDragonLair:
		return settings->GetSettingInt("iXPDiscDragonLair");
	case MarkerType::kFarm:
		return settings->GetSettingInt("iXPDiscFarm");
	case MarkerType::kWoodMill:
		return settings->GetSettingInt("iXPDiscWoodMill");
	case MarkerType::kMine:
		return settings->GetSettingInt("iXPDiscMine");
	case MarkerType::kImperialCamp:
	case MarkerType::kStormcloakCamp:
		return settings->GetSettingInt("iXPDiscMilitaryCamp");
	case MarkerType::kDoomstone:
		return settings->GetSettingInt("iXPDiscDoomstone");
	case MarkerType::kWheatMill:
		return settings->GetSettingInt("iXPDiscWheatMill");
	case MarkerType::kSmelter:
		return settings->GetSettingInt("iXPDiscSmelter");
	case MarkerType::kStable:
		return settings->GetSettingInt("iXPDiscStable");
	case MarkerType::kImperialTower:
		return settings->GetSettingInt("iXPDiscImperialTower");
	case MarkerType::kClearing:
		return settings->GetSettingInt("iXPDiscClearing");
	case MarkerType::kPass:
		return settings->GetSettingInt("iXPDiscPass");
	case MarkerType::kAltar:
		return settings->GetSettingInt("iXPDiscAltar");
	case MarkerType::kRock:
		return settings->GetSettingInt("iXPDiscRock");
	case MarkerType::kLighthouse:
		return settings->GetSettingInt("iXPDiscLighhouse");
	case MarkerType::kOrcStronghold:
		return settings->GetSettingInt("iXPDiscOrcStronghold");
	case MarkerType::kGiantCamp:
		return settings->GetSettingInt("iXPDiscGiantCamp");
	case MarkerType::kShack:
		return settings->GetSettingInt("iXPDiscShack");
	case MarkerType::kNordicTower:
		return settings->GetSettingInt("iXPDiscNordicTower");
	case MarkerType::kNordicDwelling:
		return settings->GetSettingInt("iXPDiscNordicDwelling");
	case MarkerType::kDocks:
		return settings->GetSettingInt("iXPDiscDocks");
	case MarkerType::kShrine:
		return settings->GetSettingInt("iXPDiscDaedricShrine");
	case MarkerType::kRiftenCastle:
	case MarkerType::kWindhelmCastle:
	case MarkerType::kWhiterunCastle:
	case MarkerType::kSolitudeCastle:
	case MarkerType::kMarkarthCastle:
	case MarkerType::kWinterholdCastle:
	case MarkerType::kMorthalCastle:
	case MarkerType::kFalkreathCastle:
	case MarkerType::kDawnstarCastle:
		return settings->GetSettingInt("iXPDiscCastle");
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
		return settings->GetSettingInt("iXPDiscCity");
	case MarkerType::kDLC02MiraakTemple:		
		return settings->GetSettingInt("iXPDiscMiraakTemple");
	case MarkerType::kDLC02StandingStone:
		return settings->GetSettingInt("iXPDiscStandingStone");
	case MarkerType::kDLC02TelvanniTower:	
		return settings->GetSettingInt("iXPDiscTelvanniTower");
	//case MarkerType::kDLC02ToSkyrim:
	//case MarkerType::kDLC02ToSolstheim:
	case MarkerType::kDLC02CastleKarstaag:
		return settings->GetSettingInt("iXPDiscCastleKarstaag");
	default:
		return settings->GetSettingInt("iXPDiscDefault");
	}
}
