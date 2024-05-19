#include "Events/LocationCleared.h"

#include "Settings.h"
#include "HUD.h"

#include "Hooks/BGSLocation.h"
#include "Utils/GameSettings.h"

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
	if (location && location->IsLoaded()) {

		auto type = location->GetMapMarkerType();
		auto name = location->GetName();

		ShowLocationCleared(name);

		logger::info("[LocationCleared] {0} ({1})", 
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
		return settings->GetValue<int>("iXPClearCity");
	case MarkerType::kTown:
		return settings->GetValue<int>("iXPClearTown");
	case MarkerType::kSettlement:
		return settings->GetValue<int>("iXPClearSettlement");
	case MarkerType::kCave:
		return settings->GetValue<int>("iXPClearCave");
	case MarkerType::kCamp:
		return settings->GetValue<int>("iXPClearCamp");
	case MarkerType::kFort:
		return settings->GetValue<int>("iXPClearFort");
	case MarkerType::kNordicRuin:
		return settings->GetValue<int>("iXPClearNordicRuin");
	case MarkerType::kDwemerRuin:
		return settings->GetValue<int>("iXPClearDwemerRuin");
	case MarkerType::kShipwreck:
		return settings->GetValue<int>("iXPClearShipwreck");
	case MarkerType::kGrove:
		return settings->GetValue<int>("iXPClearGrove");
	case MarkerType::kLandmark:
		return settings->GetValue<int>("iXPClearLandmark");
	case MarkerType::kDragonLair:
		return settings->GetValue<int>("iXPClearDragonLair");
	case MarkerType::kFarm:
		return settings->GetValue<int>("iXPClearFarm");
	case MarkerType::kWoodMill:
		return settings->GetValue<int>("iXPClearWoodMill");
	case MarkerType::kMine:
		return settings->GetValue<int>("iXPClearMine");
	case MarkerType::kImperialCamp:
	case MarkerType::kStormcloakCamp:
		return settings->GetValue<int>("iXPClearMilitaryCamp");
	case MarkerType::kDoomstone:
		return settings->GetValue<int>("iXPClearDoomstone");
	case MarkerType::kWheatMill:
		return settings->GetValue<int>("iXPClearWheatMill");
	case MarkerType::kSmelter:
		return settings->GetValue<int>("iXPClearSmelter");
	case MarkerType::kStable:
		return settings->GetValue<int>("iXPClearStable");
	case MarkerType::kImperialTower:
		return settings->GetValue<int>("iXPClearImperialTower");
	case MarkerType::kClearing:
		return settings->GetValue<int>("iXPClearClearing");
	case MarkerType::kPass:
		return settings->GetValue<int>("iXPClearPass");
	case MarkerType::kAltar:
		return settings->GetValue<int>("iXPClearAltar");
	case MarkerType::kRock:
		return settings->GetValue<int>("iXPClearRock");
	case MarkerType::kLighthouse:
		return settings->GetValue<int>("iXPClearLighthouse");
	case MarkerType::kOrcStronghold:
		return settings->GetValue<int>("iXPClearOrcStronghold");
	case MarkerType::kGiantCamp:
		return settings->GetValue<int>("iXPClearGiantCamp");
	case MarkerType::kShack:
		return settings->GetValue<int>("iXPClearShack");
	case MarkerType::kNordicTower:
		return settings->GetValue<int>("iXPClearNordicTower");
	case MarkerType::kNordicDwelling:
		return settings->GetValue<int>("iXPClearNordicDwelling");
	case MarkerType::kDocks:
		return settings->GetValue<int>("iXPClearDocks");
	case MarkerType::kShrine:
		return settings->GetValue<int>("iXPClearDaedricShrine");
	case MarkerType::kRiftenCastle:
	case MarkerType::kWindhelmCastle:
	case MarkerType::kWhiterunCastle:
	case MarkerType::kSolitudeCastle:
	case MarkerType::kMarkarthCastle:
	case MarkerType::kWinterholdCastle:
	case MarkerType::kMorthalCastle:
	case MarkerType::kFalkreathCastle:
	case MarkerType::kDawnstarCastle:
		return settings->GetValue<int>("iXPClearCastle");
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
		return settings->GetValue<int>("iXPClearCity");
	case MarkerType::kDLC02MiraakTemple:
		return settings->GetValue<int>("iXPClearMiraakTemple");
	case MarkerType::kDLC02StandingStone:
		return settings->GetValue<int>("iXPClearStandingStone");
	case MarkerType::kDLC02TelvanniTower:
		return settings->GetValue<int>("iXPClearTelvanniTower");
	//case MarkerType::kDLC02ToSkyrim:
	//case MarkerType::kDLC02ToSolstheim:	
	case MarkerType::kDLC02CastleKarstaag:
		return settings->GetValue<int>("iXPClearCastleKarstaag");
	default:
		return settings->GetValue<int>("iXPClearDefault");
	}
}

void LocationClearedEventHandler::ShowLocationCleared(const char* text)
{
	auto status = Utils::GetGameSettingString("sCleared");

	HUD::ShowNotification(text, status, "", HUD::MessageType::kQuestStarted);
}
