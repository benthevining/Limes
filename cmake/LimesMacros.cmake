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

if((NOT LIMES_ROOT_DIR) OR NOT IS_DIRECTORY "${LIMES_ROOT_DIR}")
	message (FATAL_ERROR "Limes root directory ${LIMES_ROOT_DIR} not specified or does not exist!")
endif()

list (APPEND CMAKE_MODULE_PATH "${LIMES_ROOT_DIR}/cmake")

list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

set (LIMES_CMAKE_MODULE_PATH "${LIMES_ROOT_DIR}/cmake" CACHE INTERNAL "")
