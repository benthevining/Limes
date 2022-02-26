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

if(NOT DEFINED ENV{CPM_SOURCE_CACHE})
	if(Lemons_SOURCE_DIR)
		set (FETCHCONTENT_BASE_DIR "${Lemons_SOURCE_DIR}/Cache" CACHE INTERNAL "")
	else()
		set (FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Cache" CACHE INTERNAL "")
	endif()
else()
	set (FETCHCONTENT_BASE_DIR "$ENV{CPM_SOURCE_CACHE}" CACHE INTERNAL "")
endif()

include (FetchContent)

FetchContent_Declare (Oranges GIT_REPOSITORY https://github.com/benthevining/Oranges.git
					  GIT_TAG origin/main)

FetchContent_MakeAvailable (Oranges)
