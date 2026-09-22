#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#pragma warning(push)

#include "RE/Skyrim.h"
#include "REX/REX.h"
#include "SKSE/SKSE.h"

#include <spdlog/spdlog.h>

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif

#include <SimpleIni.h>
#include <magic_enum/magic_enum.hpp>

#pragma warning(pop)

using namespace std::literals;

namespace logger = SKSE::log;

#define SKSE_MAKE_SOURCE_LOGGER(a_func, a_type)                           \
                                                                          \
	template <class... Args>                                              \
	struct [[maybe_unused]] a_func                                        \
	{                                                                     \
		a_func() = delete;                                                \
                                                                          \
		explicit a_func(                                                  \
			spdlog::format_string_t<Args...> a_fmt,                       \
			Args&&... a_args,                                             \
			std::source_location a_loc = std::source_location::current()) \
		{                                                                 \
			spdlog::log(                                                  \
				spdlog::source_loc {                                      \
					a_loc.file_name(),                                    \
					static_cast<int>(a_loc.line()),                       \
					a_loc.function_name() },                              \
				spdlog::level::a_type,                                    \
				a_fmt,                                                    \
				std::forward<Args>(a_args)...);                           \
		}                                                                 \
	};                                                                    \
                                                                          \
	template <class... Args>                                              \
	a_func(spdlog::format_string_t<Args...>, Args&&...) -> a_func<Args...>;

namespace SKSE::log
{
	SKSE_MAKE_SOURCE_LOGGER(trace, trace);
	SKSE_MAKE_SOURCE_LOGGER(debug, debug);
	SKSE_MAKE_SOURCE_LOGGER(info, info);
	SKSE_MAKE_SOURCE_LOGGER(warn, warn);
	SKSE_MAKE_SOURCE_LOGGER(error, err);
	SKSE_MAKE_SOURCE_LOGGER(critical, critical);
}

#undef SKSE_MAKE_SOURCE_LOGGER

#ifdef SKYRIM_SUPPORT_AE
#	define OFFSET(se, ae, vr) ae
#elif SKYRIMVR
#	define OFFSET(se, ae, vr) vr
#else
#	define OFFSET(se, ae, vr) se
#endif

template <typename EnumType>
std::string EnumToString(EnumType enumValue)
{
	return std::string(magic_enum::enum_name(enumValue));
}

#include "Plugin.h"
