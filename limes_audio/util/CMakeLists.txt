# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

target_sources (limes_audio PRIVATE Windowing.h PitchDetector.h PitchDetector.cpp
									MonoStereoConverter.h MonoStereoConverter.cpp)

target_include_directories (limes_audio PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
