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

include (FetchContent)

FetchContent_Declare (Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git
					  GIT_TAG origin/devel)

FetchContent_MakeAvailable (Catch2)

set (Catch2_FOUND TRUE)
