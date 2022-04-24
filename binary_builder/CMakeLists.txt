# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

#[[
TO DO:
- in generated cpp, export symbol / header
- in CMake module, check if passed outfilenames end in .h
]]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (CPackComponent)

cpack_add_component (limes_binary_builder_runtime GROUP limes_binary_builder INSTALL_TYPES User)

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

	set (binary_dir "${CMAKE_CURRENT_BINARY_DIR}/BinaryBuilder")

	execute_process (COMMAND "${CMAKE_COMMAND}"
		-B "${binary_dir}"
		-S "${CMAKE_CURRENT_LIST_DIR}"
		-G "${CMAKE_GENERATOR}"
		"-DCMAKE_SYSTEM_NAME=${CMAKE_HOST_SYSTEM_NAME}"
		"-DCMAKE_CROSSCOMPILING=OFF"
		"-DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}"
		"-DCMAKE_BUILD_TYPE=Debug"
		"-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}"
		RESULT_VARIABLE result)

	if(result)
		message (FATAL_ERROR "Configuring BinaryBuilder failed!")
	endif()

	execute_process (COMMAND "${CMAKE_COMMAND}"
		--build "${binary_dir}"
		--target BinaryBuilder
		--config Debug
		RESULT_VARIABLE result)

	if(result)
		message (FATAL_ERROR "Building BinaryBuilder failed!")
	endif()

	include ("${binary_dir}/LimesBinaryBuilder.cmake")

	include (GNUInstallDirs)

	get_target_property (imported_location Limes::BinaryBuilder IMPORTED_LOCATION_DEBUG)

	install (PROGRAMS "${imported_location}" DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT limes_binary_builder_runtime)

	unset (binary_dir)

	return()
endif()

project (
	LimesBinaryBuilder
	VERSION 4.5.0
	DESCRIPTION
		"Command-line utility to generate C++ source code with embedded binary resource files"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

if (PROJECT_IS_TOP_LEVEL)
	list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
	find_package (Oranges 2.17 REQUIRED)

	include (FeatureSummary)

	set_package_properties (Oranges PROPERTIES TYPE REQUIRED PURPOSE "Required CMake modules")

	include (OrangesDefaultProjectSettings)

	set (
	LIMES_INSTALL_DEST "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	CACHE STRING
		  "Directory below INSTALL_PREFIX where the Limes CMake package files will be installed to")
endif()

include (OrangesDefaultTarget)
include (OrangesUnityBuild)

enable_testing ()

add_executable (BinaryBuilder main.cpp)

set_target_properties (BinaryBuilder PROPERTIES VERSION "${PROJECT_VERSION}"
												SOVERSION "${PROJECT_VERSION_MAJOR}"
												MACOSX_BUNDLE OFF)

target_link_libraries (BinaryBuilder PRIVATE Oranges::OrangesDefaultTarget
											 Oranges::OrangesUnityBuild)

add_executable (Limes::BinaryBuilder ALIAS BinaryBuilder)

export (TARGETS BinaryBuilder
		NAMESPACE Limes::
		FILE "${CMAKE_BINARY_DIR}/LimesBinaryBuilder.cmake")

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

install (FILES "${CMAKE_CURRENT_LIST_DIR}/../cmake/LimesBinaryBuilder.cmake"
		 COMPONENT limes_binary_builder_dev DESTINATION "${LIMES_INSTALL_DEST}/cmake")

cpack_add_component (limes_binary_builder_base HIDDEN)

cpack_add_component (limes_binary_builder_dev GROUP limes_binary_builder
					 DEPENDS limes_binary_builder_runtime INSTALL_TYPES Developer)
