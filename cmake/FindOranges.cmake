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

include (FeatureSummary)

set_package_properties (Oranges PROPERTIES URL "https://github.com/benthevining/Oranges"
						DESCRIPTION "CMake modules and toolchains")

set (Oranges_FOUND TRUE)

set (oranges_parallel_dir "${CMAKE_CURRENT_LIST_DIR}/../../Oranges")

if(EXISTS "${oranges_parallel_dir}")
	add_subdirectory ("${oranges_parallel_dir}" "${CMAKE_CURRENT_BINARY_DIR}/Oranges")

	list (APPEND CMAKE_MODULE_PATH "${ORANGES_CMAKE_MODULE_PATH}")
	unset (oranges_parallel_dir)

	return ()
endif()

unset (oranges_parallel_dir)

set (FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/Cache" CACHE PATH "FetchContent dependency cache")

mark_as_advanced (FORCE FETCHCONTENT_BASE_DIR)

include (FetchContent)

FetchContent_Declare (Oranges GIT_REPOSITORY https://github.com/benthevining/Oranges.git
					  GIT_TAG origin/main)

FetchContent_MakeAvailable (Oranges)

list (APPEND CMAKE_MODULE_PATH "${ORANGES_CMAKE_MODULE_PATH}")
unset (oranges_parallel_dir)
