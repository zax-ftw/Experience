cmake_minimum_required(VERSION 3.21)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}")

# ########################################################################################################################
# ## Build options
# ########################################################################################################################

option(BUILD_SKYRIMSE "Build for Skyrim SE" OFF)
option(BUILD_SKYRIMAE "Build for Skyrim AE" OFF)
option(BUILD_SKYRIMVR "Build for Skyrim VR" OFF)

option(COPY_OUTPUT "Copy the build output to the Skyrim directory." ON)

########################################################################################################################
## Define project
########################################################################################################################

project(
	Experience
	VERSION 3.4.3
	LANGUAGES CXX
)

#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(SKSE_SUPPORT_XBYAK ON)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
	${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h
	@ONLY
)

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
 "src/Hooks/Actor.cpp"
 "src/Hooks/Actor.h"
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
 "src/Skyrim/HUDMenu.cpp"
 "src/Skyrim/HUDMenu.h"
 "src/Skyrim/StatsMenu.cpp"
 "src/Skyrim/StatsMenu.h"
 "src/Utils/GameSettings.h" 
 "src/Utils/DirectoryParser.h"
 "src/Utils/SafeQueue.h"
 "src/Utils/Singleton.h")

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "Source Files"
	FILES ${SOURCE_FILES}
)

source_group(
	TREE ${CMAKE_CURRENT_BINARY_DIR}
	PREFIX "Source Files"
	FILES ${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
	PREFIX "Header Files"
	FILES ${HEADER_FILES}
)

cmake_host_system_information(
	RESULT SKYRIMSE_PATH
	QUERY WINDOWS_REGISTRY
	"HKLM/SOFTWARE/WOW6432Node/Bethesda Softworks/Skyrim Special Edition"
	VALUE "Installed Path"
)

cmake_host_system_information(
	RESULT SKYRIMVR_PATH
	QUERY WINDOWS_REGISTRY
	"HKLM/SOFTWARE/WOW6432Node/Bethesda Softworks/Skyrim VR"
	VALUE "Installed Path"
)

if(BUILD_SKYRIMSE)
	set(CLIB_NAME "CommonLibSSE")
	set(COPY_PATH ${SKYRIMSE_PATH})
elseif(BUILD_SKYRIMAE)
	add_compile_definitions(SKYRIM_SUPPORT_AE)
	set(CLIB_NAME "CommonLibSSE")
	set(COPY_PATH ${SKYRIMSE_PATH})
elseif(BUILD_SKYRIMVR)
	add_compile_definitions(SKYRIMVR)
	set(CLIB_NAME "CommonLibVR")
	set(COPY_PATH ${SKYRIMVR_PATH})
endif()

set(CLIB_PATH "extern/${CLIB_NAME}")

########################################################################################################################
## Configure target DLL
########################################################################################################################

add_subdirectory(${CLIB_PATH} ${CLIB_NAME} EXCLUDE_FROM_ALL)

find_package(magic_enum CONFIG REQUIRED)
find_package(xbyak REQUIRED CONFIG)

find_path(SIMPLEINI_INCLUDE_DIRS "ConvertUTF.c")

add_library(
	${PROJECT_NAME}
	SHARED
	${SOURCE_FILES}
	${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	.editorconfig
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	${CLIB_NAME}::${CLIB_NAME}
	magic_enum::magic_enum
	xbyak::xbyak
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SIMPLEINI_INCLUDE_DIRS}
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

# #######################################################################################################################
# # Automatic plugin deployment
# #######################################################################################################################

if(COPY_OUTPUT)
	if (NOT ${COPY_PATH} STREQUAL "")
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${COPY_PATH}/Data/SKSE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${COPY_PATH}/Data/SKSE/Plugins/
		)
	endif()
endif()
