#include "Hooks/PlayerSkills.h"
#include "Hooks/TrainingMenu.h"
#include "Hooks/BGSLocation.h"

#include "Experience.h"
#include "Serialization.h"
#include "HUDInjector.h"
#include "Settings.h"
#include "Papyrus.h"
#include "Console.h"

#include "ModAPI.h"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace Experience
{
	void InitializeLogging()
	{
		auto path = log_directory();
		if (!path) {
			report_and_fail("Unable to lookup SKSE logs directory");
		}
		*path /= PluginDeclaration::GetSingleton()->GetName();
		*path += L".log";

		std::shared_ptr<spdlog::logger> log;
		if (IsDebuggerPresent()) {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
		} else {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
		}

#ifndef NDEBUG
		const auto level = spdlog::level::trace;
#else
		const auto level = spdlog::level::info;
#endif

		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%H:%M:%S][%l] %v");
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

	extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse)
	{
		InitializeLogging();

		auto plugin = PluginDeclaration::GetSingleton();

		logger::info("{} {} is loading...", 
			plugin->GetName(), plugin->GetVersion());

		SKSE::Init(skse);
		InitializeMessaging();
		InitializeSerialization();
		InitializePapyrus();

		Settings::GetSingleton()->LoadSettings();
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
