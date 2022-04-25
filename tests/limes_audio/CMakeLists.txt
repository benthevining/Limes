# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

target_sources (LimesTests PRIVATE # cmake-format: sortable
								   audio_test_utils.h circularBuffer_tests.cpp)

target_include_directories (LimesTests PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
