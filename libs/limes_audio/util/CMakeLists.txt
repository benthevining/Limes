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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchDetector.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MonoStereoConverter.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Windowing.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchDetector.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MonoStereoConverter.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Misc.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CircularBuffer.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CircularBuffer.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PerlinNoise.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PerlinNoise.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/Windowing.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/PitchDetector.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/MonoStereoConverter.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/Misc.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/CircularBuffer.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/PerlinNoise.h>)

install (
	FILES "${CMAKE_CURRENT_LIST_DIR}/Windowing.h" "${CMAKE_CURRENT_LIST_DIR}/PitchDetector.h"
		  "${CMAKE_CURRENT_LIST_DIR}/MonoStereoConverter.h" "${CMAKE_CURRENT_LIST_DIR}/Misc.h"
		  "${CMAKE_CURRENT_LIST_DIR}/CircularBuffer.h" "${CMAKE_CURRENT_LIST_DIR}/PerlinNoise.h"
	DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util"
	COMPONENT limes_audio_dev)
