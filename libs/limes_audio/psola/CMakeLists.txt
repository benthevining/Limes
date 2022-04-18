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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PeakFinder.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PeakFinder.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/analyzer.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/analyzer.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/shifter.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/shifter.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/psola/PeakFinder.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/psola/analyzer.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/psola/shifter.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/PeakFinder.h" "${CMAKE_CURRENT_LIST_DIR}/analyzer.h"
			   "${CMAKE_CURRENT_LIST_DIR}/shifter.h"
		 DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/psola" COMPONENT limes_audio_dev)
