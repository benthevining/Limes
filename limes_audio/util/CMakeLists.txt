# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

target_sources (
	limes_audio
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Windowing.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchDetector.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchDetector.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MonoStereoConverter.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MonoStereoConverter.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Misc.h>
			$<INSTALL_INTERFACE:include/limes_audio/Windowing.h>
			$<INSTALL_INTERFACE:include/limes_audio/PitchDetector.h>
			$<INSTALL_INTERFACE:include/limes_audio/MonoStereoConverter.h>
			$<INSTALL_INTERFACE:include/limes_audio/Misc.h>)

target_include_directories (limes_audio PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
