# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

# cmake-format: off
# building command-line tools for iOS isn't supported by Apple's SDKs
if (IOS
	OR ("${CMAKE_SYSTEM_NAME}" STREQUAL iOS)
	OR ("${CMAKE_SYSTEM_NAME}" STREQUAL tvOS)
	OR ("${CMAKE_SYSTEM_NAME}" STREQUAL watchOS))
	return ()
endif ()
# cmake-format: on

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

project (
	LimesTests
	VERSION 5.0.3
	DESCRIPTION "Unit tests for Limes"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

enable_testing ()

option (LIMES_TEST_SYSTEM_INSTALL "Test a version of Limes installed on the system"
		"${PROJECT_IS_TOP_LEVEL}")

if (LIMES_TEST_SYSTEM_INSTALL)
	find_package (Limes "${PROJECT_VERSION}" EXACT REQUIRED CONFIG COMPONENTS Libs)
endif ()

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
			Oranges::OrangesDefaultWarnings)

add_subdirectory (limes_core)
add_subdirectory (limes_music)
add_subdirectory (limes_audio)

get_target_property (tests_sources LimesTests SOURCES)

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${tests_sources})

add_executable (Limes::LimesTests ALIAS LimesTests)

foreach (tag IN ITEMS core audio music)
	add_test (NAME "Limes.${tag}" COMMAND Limes::LimesTests "[${tag}]" --warn NoAssertions --order
										  rand --colour-mode ansi --verbosity high)

	set_tests_properties ("Limes.${tag}" PROPERTIES LABELS "Limes;UnitTests")
endforeach ()
