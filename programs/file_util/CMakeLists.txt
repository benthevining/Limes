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
	LimesFileUtil
	VERSION "${LimesPrograms_VERSION}"
	DESCRIPTION "Command-line utility to provide some universal file utility operations"
	HOMEPAGE_URL "${LimesPrograms_HOMEPAGE_URL}"
	LANGUAGES CXX)

include (CPackComponent)

cpack_add_component (limes_file_util GROUP limes_programs DESCRIPTION "${PROJECT_DESCRIPTION}"
					 INSTALL_TYPES Developer)

if (CMAKE_CROSSCOMPILING)

	include ("${LIMES_BINARY_DIR}/LimesFileUtil.cmake")

	include (GNUInstallDirs)

	get_target_property (imported_location Limes::FileUtil IMPORTED_LOCATION_${LIMES_BUILD_CONFIG})

	install (PROGRAMS "${imported_location}" DESTINATION "${CMAKE_INSTALL_BINDIR}"
			 COMPONENT limes_file_util_runtime)

	return ()
endif ()

add_executable (FileUtil)

target_sources (
	FileUtil
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/commandLine.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/commandLine.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fileutil.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fileutil.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/main.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/usage.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/usage.h>)

set_target_properties (
	FileUtil
	PROPERTIES VERSION "${PROJECT_VERSION}"
			   SOVERSION "${PROJECT_VERSION_MAJOR}"
			   MACOSX_BUNDLE OFF
			   LABELS "Limes;Utility"
			   FOLDER Limes/programs
			   EchoString "Building the Limes file utility...")

target_link_libraries (FileUtil PRIVATE Limes::limes_core Oranges::OrangesDefaultTarget
										$<BUILD_INTERFACE:Oranges::OrangesUnityBuild>)

export (TARGETS FileUtil NAMESPACE Limes:: FILE "${CMAKE_BINARY_DIR}/LimesFileUtil.cmake")

add_executable (Limes::FileUtil ALIAS FileUtil)

# cmake-format: off
install (
	TARGETS FileUtil
	EXPORT LimesFileUtilTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_file_util_runtime
	NAMELINK_COMPONENT limes_file_util_dev
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT limes_file_util_dev
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_file_util_runtime)
# cmake-format: on

install (
	EXPORT LimesFileUtilTargets
	NAMESPACE Limes::
	DESTINATION "${LIMES_INSTALL_DEST}"
	EXPORT_LINK_INTERFACE_LIBRARIES
	COMPONENT limes_file_util_base)

export (EXPORT LimesFileUtilTargets FILE "${LIMES_EXPORT_DIR}/LimesFileUtilTargets.cmake"
		NAMESPACE Limes::)
