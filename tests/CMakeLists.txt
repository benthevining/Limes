# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

return ()

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	LimesTests
	VERSION 4.8.0
	DESCRIPTION "Unit tests for Limes"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

enable_testing ()

if (PROJECT_IS_TOP_LEVEL)
	set (CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

	find_package (Limes "${PROJECT_VERSION}" EXACT REQUIRED CONFIG COMPONENTS Libs)

	list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
endif ()

include (OrangesDefaultTarget)
include (OrangesDefaultWarnings)
include (OrangesAllIntegrations)

find_package (Catch2 3 REQUIRED)

add_executable (LimesTests tests_config.h)

set_target_properties (
	LimesTests
	PROPERTIES LABELS "Limes;Tests" FOLDER Limes EchoString "Building the Limes unit test runner..."
			   MACOSX_BUNDLE OFF)

target_include_directories (LimesTests PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

target_link_libraries (
	LimesTests
	PRIVATE # cmake-format: sortable
			Catch2::Catch2WithMain Limes::Limes Oranges::OrangesUnityBuild
			Oranges::OrangesDefaultWarnings Oranges::OrangesAllIntegrations)

add_subdirectory (limes_core)
add_subdirectory (limes_data_structures)
add_subdirectory (limes_music)
add_subdirectory (limes_audio)

get_target_property (tests_sources LimesTests SOURCES)

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${tests_sources})

add_executable (Limes::LimesTests ALIAS LimesTests)

foreach (tag IN ITEMS core audio music data_structures)
	add_test (NAME "Limes.${tag}" COMMAND Limes::LimesTests "[${tag}]" --warn NoAssertions --order
										  rand --colour-mode ansi --verbosity high)

	set_tests_properties ("Limes.${tag}" PROPERTIES LABELS "Limes;UnitTests")
endforeach ()
