#include "Hooks/PlayerSkills.h"
#include "Hooks/TrainingMenu.h"
#include "Hooks/BGSLocation.h"
#include "Hooks/Actor.h"

#include "Experience.h"
#include "Serialization.h"
#include "HUDInjector.h"
#include "Settings.h"
#include "Papyrus.h"
#include "Console.h"
#include "ModAPI.h"

using namespace SKSE;

namespace Experience
{
	void InitializeLogging()
	{
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto path = logger::log_directory();
		if (!path) {
			stl::report_and_fail("Unable to lookup SKSE logs directory"sv);
		}

		*path /= fmt::format("{}.log"sv, Plugin::NAME);
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

		spdlog::set_default_logger(std::move(log));
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
			stl::report_and_fail("Failure to register Papyrus bindings");
		}
	}

    void InitializeHooking()
	{
		logger::trace("Initializing trampoline...");
		auto& trampoline = GetTrampoline();
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
				case SKSE::MessagingInterface::kDataLoaded:  // MainMenu
					{
						ExperienceManager::GetSingleton()->Init();
						ObScript::Init();
					}
					break;
				}
			})) {
			stl::report_and_fail("Unable to register message listener.");
		}
	}

#ifdef SKYRIM_SUPPORT_AE
	extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
		SKSE::PluginVersionData v;

		v.PluginVersion(Plugin::VERSION);
		v.PluginName(Plugin::NAME);
		v.AuthorName("Zax-Ftw");
		v.UsesAddressLibrary();
		v.UsesUpdatedStructs();
		v.CompatibleVersions({ SKSE::RUNTIME_LATEST });

		return v;
	}();
#else
	extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = Plugin::NAME.data();
		a_info->version = Plugin::VERSION.pack();

		if (a_skse->IsEditor()) {
			logger::critical("Loaded in editor, marking as incompatible"sv);
			return false;
		}

		const auto ver = a_skse->RuntimeVersion();
#	ifdef SKYRIMVR
		if (ver > SKSE::RUNTIME_VR_1_4_15_1) {
#	else
		if (ver < SKSE::RUNTIME_1_5_39) {
#	endif	
			logger::critical("Unsupported runtime version {}", ver.string());
			return false;
		}
		return true;
	}
#endif

	extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse)
	{
		InitializeLogging();

		logger::info("{} {} is loading...", 
			Plugin::NAME, Plugin::VERSION.string());

		SKSE::Init(skse);
		InitializeMessaging();
		InitializeSerialization();
		InitializePapyrus();

		Settings::GetSingleton()->ReadSettings();
		HUDInjector::GetSingleton()->Register();

		return true;
	}

	extern "C" DLLEXPORT void* SKSEAPI RequestAPI(ApiVersion a_version)
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
