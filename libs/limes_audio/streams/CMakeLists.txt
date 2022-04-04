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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/SampleStream.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/oscillator.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/basic_oscillators.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/SampleStream.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/oscillator.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/basic_oscillators.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/SampleStream.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/oscillator.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/basic_oscillators.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/SampleStream.h" "${CMAKE_CURRENT_LIST_DIR}/oscillator.h"
			   "${CMAKE_CURRENT_LIST_DIR}/basic_oscillators.h"
		 DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}" COMPONENT limes_audio_dev)
