# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	LimesTests
	VERSION 4.6.0
	DESCRIPTION "Unit tests for Limes"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

enable_testing ()

if(PROJECT_IS_TOP_LEVEL)
	find_package (Limes "${PROJECT_VERSION}" EXACT REQUIRED CONFIG)

	list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
endif()

include (OrangesDefaultTarget)
include (OrangesDefaultWarnings)

find_package (Catch2 3 REQUIRED)

add_executable (LimesTests math_tests.cpp tests_config.h)

set_target_properties (LimesTests PROPERTIES LABELS "Limes;Tests" FOLDER Limes
											 EchoString "Building the Limes unit test runner...")

target_include_directories (LimesTests PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

target_link_libraries (
	LimesTests
	PRIVATE # cmake-format: sortable
			Limes::Limes Catch2::Catch2WithMain Oranges::OrangesDefaultTarget
			Oranges::OrangesDefaultWarnings)

add_subdirectory (limes_music)
add_subdirectory (limes_audio)

get_target_property (tests_sources LimesTests SOURCES)

source_group (TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${tests_sources})

add_executable (Limes::LimesTests ALIAS LimesTests)

foreach(tag IN ITEMS core audio music)
	add_test (NAME Limes.UnitTests.${tag}
			  COMMAND Limes::LimesTests [${tag}] --success --warn NoAssertions --order rand
					  --colour-mode ansi --verbosity high)

	set_tests_properties (Limes.UnitTests.${tag} PROPERTIES LABELS "Limes;UnitTests")
endforeach()
