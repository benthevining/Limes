# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

#[=======================================================================[.rst:

LimesPrograms
-------------------------

A collection of small command line utility programs.

Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_core

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::BinaryBuilder
- Limes::FileUtil

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_binary_builder_dev
- limes_binary_builder_runtime
- limes_file_util_dev
- limes_file_util_runtime
- limes_programs_dev
- limes_programs_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_binary_builder
- limes_file_util
- limes_programs (aggregate)

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	LimesPrograms
	VERSION 4.8.1
	DESCRIPTION "Small command-line utility programs"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

enable_testing ()

include (GNUInstallDirs)
include (CPackComponent)

if (PROJECT_IS_TOP_LEVEL)
	set (
		LIMES_INSTALL_DEST
		"${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		CACHE
			STRING
			"Directory below INSTALL_PREFIX where the Limes CMake package files will be installed to"
		)

	set (limes_root "${CMAKE_CURRENT_LIST_DIR}/..")

	list (APPEND CMAKE_MODULE_PATH "${limes_root}/cmake")

	find_package (Oranges 2.24.0 REQUIRED)

	set (Limes_VERSION "${PROJECT_VERSION}")

	add_subdirectory ("${limes_root}/libs" "${CMAKE_BINARY_DIR}/libs")
endif ()

#

if (CMAKE_CROSSCOMPILING)

	# we need to build the programs for the HOST system, not the TARGET system

	unset (ENV{ASM})
	unset (ENV{CC})
	unset (ENV{CXX})

	set (LIMES_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/host_subbuild")

	if (CMAKE_BUILD_TYPE)
		set (LIMES_BUILD_CONFIG "${CMAKE_BUILD_TYPE}")
	else ()
		set (LIMES_BUILD_CONFIG Debug)
	endif ()

	# cmake-format:off
	execute_process (
		COMMAND
			"${CMAKE_COMMAND}" -S "${CMAKE_CURRENT_LIST_DIR}" -B "${LIMES_BINARY_DIR}" -G
			"${CMAKE_GENERATOR}" -D "CMAKE_SYSTEM_NAME=${CMAKE_HOST_SYSTEM_NAME}" -D
			"CMAKE_CROSSCOMPILING=OFF" -D "CMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}" -D
			"CMAKE_BUILD_TYPE=${LIMES_BUILD_CONFIG}" -D
			"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}" -D
			"CMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}" -D
			"CMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}" -D
			"CMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}" COMMAND_ERROR_IS_FATAL ANY)
	# cmake-format:on

	execute_process (COMMAND "${CMAKE_COMMAND}" --build "${LIMES_BINARY_DIR}" --config
							 "${LIMES_BUILD_CONFIG}" COMMAND_ERROR_IS_FATAL ANY)

	string (TOUPPER "${LIMES_BUILD_CONFIG}" LIMES_BUILD_CONFIG)
endif ()

#

add_subdirectory (binary_builder)
add_subdirectory (file_util)

#

cpack_add_component (limes_programs_runtime GROUP limes_programs
					 DEPENDS limes_binary_builder_runtime INSTALL_TYPES User)

cpack_add_component (
	limes_programs_dev GROUP limes_programs DEPENDS limes_programs_runtime limes_binary_builder_dev
	INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_programs
	DISPLAY_NAME "Limes utility programs"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	BOLD_TITLE
	PARENT_GROUP limes)
