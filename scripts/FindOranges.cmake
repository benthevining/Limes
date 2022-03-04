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

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

if(DEFINED ENV{CPM_SOURCE_CACHE})
	set (FETCHCONTENT_BASE_DIR "$ENV{CPM_SOURCE_CACHE}" CACHE PATH "FetchContent dependency cache")
else()
	set (FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Cache" CACHE PATH
																 "FetchContent dependency cache")
endif()

mark_as_advanced (FORCE FETCHCONTENT_BASE_DIR)

include (FetchContent)

FetchContent_Declare (Oranges GIT_REPOSITORY https://github.com/benthevining/Oranges.git
					  GIT_TAG origin/main)

FetchContent_MakeAvailable (Oranges)

list (APPEND CMAKE_MODULE_PATH "${ORANGES_CMAKE_MODULE_PATH}")
