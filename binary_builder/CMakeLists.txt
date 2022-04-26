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

LimesBinaryBuilder
-------------------------

A command line utility to embed binary resource files into generated C++ source code.

Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_core

Targets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- Limes::BinaryBuilder

Install components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_binary_builder_dev
- limes_binary_builder_runtime

Install component groups
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
- limes_binary_builder

#]=======================================================================]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (CPackComponent)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/../cmake/LimesBinaryBuilder.cmake"
		 COMPONENT limes_binary_builder_dev DESTINATION "${LIMES_INSTALL_DEST}/cmake")

cpack_add_component (limes_binary_builder_base HIDDEN)

cpack_add_component (limes_binary_builder_runtime GROUP limes_binary_builder
					 DEPENDS limes_binary_builder_base INSTALL_TYPES User)

cpack_add_component (limes_binary_builder_dev GROUP limes_binary_builder
					 DEPENDS limes_binary_builder_runtime INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_binary_builder
	DISPLAY_NAME "Limes binary builder"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	BOLD_TITLE
	PARENT_GROUP limes)

if(CMAKE_CROSSCOMPILING)

	# we need to build the BinaryBuilder for the HOST system, not the TARGET system

	unset (ENV{ASM})
	unset (ENV{CC})
	unset (ENV{CXX})

	set (binary_dir "${CMAKE_CURRENT_BINARY_DIR}/subbuild")

	if(CMAKE_BUILD_TYPE)
		set (build_config "${CMAKE_BUILD_TYPE}")
	else()
		set (build_config Debug)
	endif()

	execute_process (
		COMMAND
			"${CMAKE_COMMAND}" -B "${binary_dir}" -S "${CMAKE_CURRENT_LIST_DIR}" -G
			"${CMAKE_GENERATOR}" -D "CMAKE_SYSTEM_NAME=${CMAKE_HOST_SYSTEM_NAME}" -D
			"CMAKE_CROSSCOMPILING=OFF" -D "CMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}" -D
			"CMAKE_BUILD_TYPE=${build_config}" -D "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}"
			COMMAND_ERROR_IS_FATAL ANY)

	execute_process (COMMAND "${CMAKE_COMMAND}" --build "${binary_dir}" --target BinaryBuilder
							 --config "${build_config}" COMMAND_ERROR_IS_FATAL ANY)

	include ("${binary_dir}/LimesBinaryBuilder.cmake")

	include (GNUInstallDirs)

	string (TOUPPER "${build_config}" build_config)

	get_target_property (imported_location Limes::BinaryBuilder IMPORTED_LOCATION_${build_config})

	install (PROGRAMS "${imported_location}" DESTINATION "${CMAKE_INSTALL_BINDIR}"
			 COMPONENT limes_binary_builder_runtime)

	unset (binary_dir)
	unset (build_config)

	return ()
endif()

project (
	LimesBinaryBuilder
	VERSION 4.5.0
	DESCRIPTION
		"Command-line utility to generate C++ source code with embedded binary resource files"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

include (OrangesDefaultTarget)
include (OrangesUnityBuild)

enable_testing ()

add_executable (BinaryBuilder main.cpp)

set_target_properties (
	BinaryBuilder
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   MACOSX_BUNDLE OFF
			   LABELS "Limes;Utility"
			   FOLDER Limes
			   EchoString "Building the Limes BinaryBuilder...")

target_link_libraries (BinaryBuilder PRIVATE Limes::limes_core Oranges::OrangesDefaultTarget
											 Oranges::OrangesUnityBuild)

add_executable (Limes::BinaryBuilder ALIAS BinaryBuilder)

export (TARGETS BinaryBuilder NAMESPACE Limes:: FILE "${CMAKE_BINARY_DIR}/LimesBinaryBuilder.cmake")

install (
	TARGETS BinaryBuilder
	EXPORT LimesBinaryBuilderTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_binary_builder_runtime
			NAMELINK_COMPONENT limes_binary_builder_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_binary_builder_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_binary_builder_runtime
	INCLUDES
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install (
	EXPORT LimesBinaryBuilderTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_binary_builder_base)
