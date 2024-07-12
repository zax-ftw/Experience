#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#pragma warning(push)

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif

#include <SimpleIni.h>
#include <magic_enum.hpp>
#include <xbyak/xbyak.h>

#pragma warning(pop)

// Compatible declarations with other sample projects.
#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;

namespace logger = SKSE::log;

#ifdef SKYRIM_SUPPORT_AE
#	define OFFSET(se, ae, vr) ae
#elif SKYRIMVR
#	define OFFSET(se, ae, vr) vr
#else
#	define OFFSET(se, ae, vr) se
#endif

#include "Plugin.h"
