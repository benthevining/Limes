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

project (
	LimesBinaryBuilder
	VERSION 4.5.0
	DESCRIPTION
		"Command-line utility to generate C++ source code with embedded binary resource files"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

enable_testing ()

add_executable (BinaryBuilder main.cpp)

set_target_properties (BinaryBuilder PROPERTIES VERSION "${PROJECT_VERSION}"
												SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (BinaryBuilder PRIVATE Oranges::OrangesDefaultTarget
											 Oranges::OrangesUnityBuild)

add_executable (Limes::BinaryBuilder ALIAS BinaryBuilder)

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

cpack_add_component (limes_binary_builder_runtime GROUP limes_binary_builder INSTALL_TYPES User)

cpack_add_component (limes_binary_builder_dev GROUP limes_binary_builder
					 DEPENDS limes_binary_builder_runtime INSTALL_TYPES Developer)

cpack_add_component_group (
	limes_binary_builder
	DISPLAY_NAME "Limes binary builder"
	DESCRIPTION "${PROJECT_DESCRIPTION}"
	BOLD_TITLE
	PARENT_GROUP limes)
