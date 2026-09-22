#include "Hooks/PlayerSkills.h"
#include "Hooks/TrainingMenu.h"
#include "Hooks/BGSLocation.h"
#include "Hooks/Actor.h"

#include "Experience.h"
#include "Serialization.h"
#include "HUDInjector.h"
#include "Settings/Settings.h"
#include "Papyrus.h"
#include "Console.h"
#include "ModAPI.h"

using namespace SKSE;

namespace Experience
{
	constexpr REL::Version SSE_1_7_99(1, 7, 99, 0);

	void InitializeLogging()
	{
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto path = logger::log_directory();
		if (!path) {
			REX::FAIL("Unable to lookup SKSE logs directory"sv);
		}

		*path /= std::format("{}.log"sv, Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
		const auto level = spdlog::level::trace;
#else
		const auto level = spdlog::level::info;
#endif

		auto log = std::make_shared<spdlog::logger>("Global"s, std::move(sink));

		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(log);
		spdlog::set_pattern("[%H:%M:%S][%l] %v"s);
	}

    void InitializeSerialization()
	{
		logger::trace("Initializing cosave serialization...");
		auto* serde = GetSerializationInterface();
		serde->SetUniqueID('EXPE');
		serde->SetRevertCallback(Serialization::RevertCallback);
		serde->SetSaveCallback(Serialization::SaveCallback);
		serde->SetLoadCallback(Serialization::LoadCallback);
		logger::trace("Cosave serialization initialized");
	}

	void InitializePapyrus()
	{
		logger::trace("Initializing Papyrus binding...");
		if (GetPapyrusInterface()->Register(Papyrus::RegisterFuncs)) {
			logger::debug("Papyrus functions bound");
		} else {
			REX::FAIL("Failure to register Papyrus bindings");
		}
	}

    void InitializeHooking()
	{
		logger::trace("Initializing trampoline...");
		auto& trampoline = REL::GetTrampoline();
		trampoline.create(256);
		logger::trace("Trampoline initialized");

		PlayerSkillsEx::Install(trampoline);
		TrainingMenuEx::Install(trampoline);
		BGSLocationEx::Install(trampoline);
		ActorEx::Install(trampoline);
	}

    void InitializeMessaging()
	{
		if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
				switch (message->type) {
				case SKSE::MessagingInterface::kPostLoad:
					{
						InitializeHooking();
					}
					break;
				case SKSE::MessagingInterface::kPostLoadGame:
					{
						PlayerSkillsEx::PostLoad();
					}
					break;
				case SKSE::MessagingInterface::kDataLoaded:  // MainMenu
					{
						ExperienceManager::GetSingleton()->Init();
						ObScript::Init();
					}
					break;
				}
			})) {
			REX::FAIL("Unable to register message listener.");
		}
	}

#ifdef SKYRIM_SUPPORT_AE
	SKSE_PLUGIN_VERSION = []() {
		SKSE::PluginVersionData v;

		v.PluginVersion(Plugin::VERSION);
		v.PluginName(Plugin::NAME);
		v.AuthorName("Zax-Ftw");
		v.UsesAddressLibrary();
		v.UsesUpdatedStructs();
		v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });

		if constexpr (SKSE::RUNTIME_SSE_LATEST < SSE_1_7_99) {
			v.MinimumRequiredXSEVersion({ 2, 2, 5 });
		} else {
			v.MinimumRequiredXSEVersion({ 2, 3, 0 });
		}

		return v;
	}();
#else
	SKSE_PLUGIN_QUERY(const SKSE::QueryInterface* skse, SKSE::PluginInfo* info)
	{
		info->infoVersion = SKSE::PluginInfo::kVersion;
		info->name = Plugin::NAME.data();
		info->version = Plugin::VERSION.pack();

		if (skse->IsEditor()) {
			REX::CRITICAL("Loaded in editor, marking as incompatible"sv);
			return false;
		}

		const auto ver = skse->RuntimeVersion();
#	ifdef SKYRIMVR
		if (ver > SKSE::RUNTIME_VR_1_4_15_1) {
#	else
		if (ver < SKSE::RUNTIME_SSE_1_5_39) {
#	endif	
			REX::CRITICAL("Unsupported runtime version {}", ver.string());
			return false;
		}
		return true;
	}
#endif

	SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* skse)
	{
		InitializeLogging();

		REX::INFO("{} {} is loading...", 
			Plugin::NAME, Plugin::VERSION.string("."));

		SKSE::Init(skse, false);

		const auto runtime = skse->RuntimeVersion();
		REX::INFO("Game version: {}", runtime);

#ifdef SKYRIM_SUPPORT_AE
		if constexpr (SKSE::RUNTIME_SSE_LATEST < SSE_1_7_99) {
			if (runtime >= SSE_1_7_99) {
				REX::FAIL("{} does not support Skyrim runtime {}.", Plugin::NAME, runtime);
			}
		}
#endif
		InitializeMessaging();
		InitializeSerialization();
		InitializePapyrus();

		Settings::GetSingleton()->LoadSettings();
		HUDInjector::GetSingleton()->Register();

		return true;
	}

	SKSE_EXPORT void* RequestAPI(ApiVersion a_version)
	{
		auto api = ExperienceInterface::GetSingleton();

		switch (a_version) {
		case ApiVersion::Current:
			return static_cast<void*>(api);	
		}

		logger::warn("RequestAPI requested wrong interface version");
		return nullptr;
	}

}
