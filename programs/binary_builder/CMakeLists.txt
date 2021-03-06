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

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

project (
	LimesBinaryBuilder
	VERSION "${LimesPrograms_VERSION}"
	DESCRIPTION
		"Command-line utility to generate C++ source code with embedded binary resource files"
	HOMEPAGE_URL "${LimesPrograms_HOMEPAGE_URL}"
	LANGUAGES CXX)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/../../cmake/LimesBinaryBuilder.cmake"
		 COMPONENT limes_binary_builder_dev DESTINATION "${LIMES_INSTALL_DEST}/cmake")

include (CPackComponent)

cpack_add_component (limes_binary_builder GROUP limes_programs DESCRIPTION "${PROJECT_DESCRIPTION}"
					 INSTALL_TYPES Developer)

if (CMAKE_CROSSCOMPILING)

	include ("${LIMES_BINARY_DIR}/LimesBinaryBuilder.cmake")

	include (GNUInstallDirs)

	get_target_property (imported_location Limes::BinaryBuilder
						 IMPORTED_LOCATION_${LIMES_BUILD_CONFIG})

	install (PROGRAMS "${imported_location}" DESTINATION "${CMAKE_INSTALL_BINDIR}"
			 COMPONENT limes_binary_builder_runtime)

	# install (CODE "execute_process (COMMAND ${CMAKE_COMMAND} --install ${LIMES_BINARY_DIR})"
	# COMPONENT limes_binary_builder_base)

	return ()
endif ()

add_executable (BinaryBuilder)

target_sources (
	BinaryBuilder
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/commandLine.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/commandLine.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/generator.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/generator.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/main.cpp>)

set_target_properties (
	BinaryBuilder
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   MACOSX_BUNDLE OFF
			   LABELS "Limes;Utility"
			   FOLDER Limes/programs
			   EchoString "Building the Limes BinaryBuilder...")

target_link_libraries (BinaryBuilder PRIVATE Limes::limes_core Oranges::OrangesDefaultTarget
											 $<BUILD_INTERFACE:Oranges::OrangesUnityBuild>)

export (TARGETS BinaryBuilder NAMESPACE Limes:: FILE "${CMAKE_BINARY_DIR}/LimesBinaryBuilder.cmake")

add_executable (Limes::BinaryBuilder ALIAS BinaryBuilder)

# cmake-format: off
install (
	TARGETS BinaryBuilder
	EXPORT LimesBinaryBuilderTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_binary_builder_runtime
	NAMELINK_COMPONENT limes_binary_builder_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_binary_builder_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_binary_builder_runtime)
# cmake-format: on

install (
	EXPORT LimesBinaryBuilderTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_binary_builder_base)

export (EXPORT LimesBinaryBuilderTargets FILE "${LIMES_EXPORT_DIR}/LimesBinaryBuilderTargets.cmake"
		NAMESPACE Limes::)
