# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

# ======================================================================================
# ____  _____            _   _  _____ ______  _____ / __ \|  __ \     /\   | \ | |/ ____|  ____|/
# ____| | |  | | |__) |   /  \  |  \| | |  __| |__  | (___ | |  | |  _  /   / /\ \ | . ` | | |_ |
# __|  \___ \ | |__| | | \ \  / ____ \| |\  | |__| | |____ ____) | \____/|_|  \_\/_/    \_\_|
# \_|\_____|______|_____/
#
# This file is part of the Oranges open source CMake library and is licensed under the terms of the
# GNU Public License.
#
# ======================================================================================

if(NOT ORANGES_PROJECT_ROOT)
	message (FATAL_ERROR "ORANGES_PROJECT_ROOT must be defined with -D")
endif()

set (ORANGES_BUILD_ROOT "${ORANGES_PROJECT_ROOT}/Builds")
set (ORANGES_CACHE_ROOT "${ORANGES_PROJECT_ROOT}/Cache")

set (api_base_dir "${ORANGES_BUILD_ROOT}/.cmake/api/v1")

set (query_dir "${api_base_dir}/query")
set (reply_dir "${api_base_dir}/reply")

file (MAKE_DIRECTORY "${query_dir}")
file (MAKE_DIRECTORY "${reply_dir}")

configure_file ("${CMAKE_CURRENT_LIST_DIR}/query.json" "${ORANGES_CACHE_ROOT}/query.json" @ONLY)

file (COPY "${ORANGES_CACHE_ROOT}/query.json" DESTINATION "${query_dir}/client-Oranges"
	  FOLLOW_SYMLINK_CHAIN)
