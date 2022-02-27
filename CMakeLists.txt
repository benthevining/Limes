# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

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

add_subdirectory (limes_vecops)
add_subdirectory (limes_core)
add_subdirectory (limes_audio)
add_subdirectory (limes_music)

# install files

include (GNUInstallDirs)
include (CMakePackageConfigHelpers)

set (installDest ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/limes_core" DESTINATION ${installDest}
		 COMPONENT ${PROJECT_NAME})
install (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/limes_music" DESTINATION ${installDest}
		 COMPONENT ${PROJECT_NAME})
install (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/limes_audio" DESTINATION ${installDest}
		 COMPONENT ${PROJECT_NAME})
install (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/limes_vecops" DESTINATION ${installDest}
		 COMPONENT ${PROJECT_NAME})

write_basic_package_version_file ("${PROJECT_NAME}ConfigVersion.cmake" VERSION ${PROJECT_VERSION}
								  COMPATIBILITY SameMajorVersion)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		 DESTINATION ${installDest} COMPONENT ${PROJECT_NAME})
