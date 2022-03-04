include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

project (
	Limes
	VERSION 0.1.0
	DESCRIPTION "C++ utilities and building blocks"
	HOMEPAGE_URL "www.github.com/benthevining/Limes"
	LANGUAGES CXX)

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

list (APPEND CMAKE_MODULE_PATH "${LEMONS_CMAKE_MODULE_PATH}")

#

set (CMAKE_DEBUG_POSTFIX -d)

add_library (Limes INTERFACE)

include (GNUInstallDirs)

install (
	TARGETS Limes
	EXPORT LimesTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	INCLUDES
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

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

install (EXPORT LimesTargets FILE LimesTargets.cmake NAMESPACE Limes:: DESTINATION lib/cmake/Limes)
export (EXPORT LimesTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/LimesTargets.cmake"
		NAMESPACE Limes::)

include (CMakePackageConfigHelpers)

write_basic_package_version_file ("LimesConfigVersion.cmake" VERSION "${PROJECT_VERSION}"
								  COMPATIBILITY SameMajorVersion)

configure_package_config_file (
	"${CMAKE_CURRENT_LIST_DIR}/scripts/LimesConfig.cmake" "LimesConfig.cmake"
	INSTALL_DESTINATION lib/cmake/Limes PATH_VARS "" INSTALL_PREFIX "")

if(PROJECT_IS_TOP_LEVEL)
	include (CPack)
endif()
