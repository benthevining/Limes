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
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/psola/PeakFinder.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/PeakFinder.h"
		 DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/psola" COMPONENT limes_audio_dev)
