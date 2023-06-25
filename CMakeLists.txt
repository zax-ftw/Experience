cmake_minimum_required(VERSION 3.20)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}")

########################################################################################################################
## Define project
########################################################################################################################
project(
	Experience
	VERSION 3.2.0
	LANGUAGES CXX
)

set(AUTHOR_NAME "Zax-Ftw")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(SKSE_SUPPORT_XBYAK ON)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

set(HEADER_FILES
 "include/ExperienceAPI.h"
)

set(SOURCE_FILES
 "src/Main.cpp"
 "src/Settings.cpp"
 "src/Settings.h"
 "src/HUDInjector.cpp"
 "src/HUDInjector.h"
 "src/Experience.cpp"
 "src/Experience.h"
 "src/Serialization.cpp"
 "src/Serialization.h"
 "src/Translation.cpp"
 "src/Translation.h"
 "src/Papyrus.cpp"
 "src/Papyrus.h"
 "src/ModAPI.cpp"
 "src/ModAPI.h"
 "src/Console.cpp"
 "src/Console.h"
 "src/HUD.cpp"
 "src/HUD.h"
 "src/Hooks/PlayerSkills.cpp"
 "src/Hooks/PlayerSkills.h"
 "src/Hooks/TrainingMenu.cpp"
 "src/Hooks/TrainingMenu.h"
 "src/Hooks/BGSLocation.cpp"
 "src/Hooks/BGSLocation.h"
 "src/Events/ActorKill.cpp"
 "src/Events/ActorKill.h"
 "src/Events/BooksRead.cpp"
 "src/Events/BooksRead.h"
 "src/Events/LocationCleared.cpp"
 "src/Events/LocationCleared.h"
 "src/Events/LocationDiscovery.cpp"
 "src/Events/LocationDiscovery.h"
 "src/Events/ObjectiveState.cpp"
 "src/Events/ObjectiveState.h"
 "src/Events/QuestStatus.cpp"
 "src/Events/QuestStatus.h"
 "src/Events/SkillIncrease.cpp" 
 "src/Events/SkillIncrease.h"
 "src/Skyrim/StatsMenu.cpp"
 "src/Skyrim/StatsMenu.h"
 "src/Utils/SafeQueue.h" 
 "src/Utils/Singleton.h")

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "Source Files"
	FILES ${SOURCE_FILES}
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
	PREFIX "Header Files"
	FILES ${HEADER_FILES}
)

#########################################################################################################################
### Build options
#########################################################################################################################

option(BUILD_TESTS "Build unit tests." OFF)
option(COPY_OUTPUT "Copy the build output to the Skyrim directory." ON)

cmake_host_system_information(
	RESULT SKYRIM_PATH
	QUERY WINDOWS_REGISTRY 
	"HKLM/SOFTWARE/WOW6432Node/Bethesda Softworks/Skyrim Special Edition"
	VALUE "Installed Path"
)

########################################################################################################################
## Configure target DLL
########################################################################################################################

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

find_path(SIMPLEINI_INCLUDE_DIR "ConvertUTF.c")

add_library(
	${PROJECT_NAME}
	SHARED
	${SOURCE_FILES}
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	.editorconfig
)

target_commonlibsse_properties(
	${PROJECT_NAME}
	AUTHOR ${AUTHOR_NAME}
	USE_ADDRESS_LIBRARY
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	CommonLibSSE::CommonLibSSE
	magic_enum::magic_enum
)

target_include_directories(
	${PROJECT_NAME}
    PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${SIMPLEINI_INCLUDE_DIR}
)

target_include_directories(
	${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

target_compile_features(
	${PROJECT_NAME} 
	PRIVATE 
		cxx_std_23
)

########################################################################################################################
## Automatic plugin deployment
########################################################################################################################

if (COPY_OUTPUT)
	if (DEFINED SKYRIM_PATH)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${SKYRIM_PATH}/Data/SKSE/Plugins/)		
		if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
			add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${SKYRIM_PATH}/Data/SKSE/Plugins/)
		endif()
	else ()
		message("Installed Path is not defined. Skipping post-build copy command.")
	endif ()
endif ()
