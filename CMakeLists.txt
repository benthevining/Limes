include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	Limes
	VERSION 0.1.0
	DESCRIPTION "C++ utilities and building blocks"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

set (LIMES_INSTALL_DEST lib/cmake/Limes
	 CACHE STRING "Directory below INSTALL_PREFIX where Limes will be installed to")

mark_as_advanced (FORCE LIMES_INSTALL_DEST)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 10.0.0)
		message (
			FATAL_ERROR
				"This library requires C++ concepts, which are not supported on versions of GCC before 10. Your current version: ${CMAKE_CXX_COMPILER_VERSION}"
			)
	endif()
endif()

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/scripts")

find_package (Oranges REQUIRED)

#

add_library (Limes INTERFACE)

include (LemonsCmakeDevTools)

oranges_install_targets (TARGETS Limes EXPORT LimesTargets)

add_subdirectory (limes_vecops)
add_subdirectory (limes_core)
add_subdirectory (limes_audio)
add_subdirectory (limes_music)

add_library (Limes::Limes ALIAS Limes)

target_include_directories (
	limes_vecops
	INTERFACE $<BUILD_INTERFACE:${Limes_SOURCE_DIR}/include>
			  $<BUILD_INTERFACE:${Limes_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)

#

install (EXPORT LimesTargets NAMESPACE Limes:: DESTINATION "${LIMES_INSTALL_DEST}"
		 EXPORT_LINK_INTERFACE_LIBRARIES)

include (CMakePackageConfigHelpers)

write_basic_package_version_file ("LimesConfigVersion.cmake" VERSION "${PROJECT_VERSION}"
								  COMPATIBILITY SameMajorVersion)

configure_package_config_file (
	"${CMAKE_CURRENT_LIST_DIR}/scripts/LimesConfig.cmake" "LimesConfig.cmake"
	INSTALL_DESTINATION lib/cmake/Limes PATH_VARS "" INSTALL_PREFIX "")

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/LimesConfigVersion.cmake"
			   "${CMAKE_CURRENT_BINARY_DIR}/LimesConfig.cmake" DESTINATION "${LIMES_INSTALL_DEST}")

include (LemonsDefaultCPackSettings)

export (PACKAGE Limes)
