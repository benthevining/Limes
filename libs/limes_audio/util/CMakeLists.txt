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
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CircularBuffer.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CircularBuffer.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Misc.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MonoStereoConverter.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MonoStereoConverter.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PerlinNoise.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PerlinNoise.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchDetector.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchDetector.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/CircularBuffer.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/Misc.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/MonoStereoConverter.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/PerlinNoise.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util/PitchDetector.h>)

install (FILES # cmake-format: sortable
			   CircularBuffer.h Misc.h MonoStereoConverter.h PerlinNoise.h PitchDetector.h
		 DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/util" COMPONENT limes_audio_dev)

set (
	util_files
	# cmake-format: sortable
	util/CircularBuffer.cpp
	util/CircularBuffer.h
	util/Misc.h
	util/MonoStereoConverter.cpp
	util/MonoStereoConverter.h
	util/PerlinNoise.cpp
	util/PerlinNoise.h
	util/PitchDetector.cpp
	util/PitchDetector.h
	PARENT_SCOPE)
