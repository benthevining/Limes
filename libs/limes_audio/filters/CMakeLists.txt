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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Coeffecients.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Coeffecients.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Filter.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Filter.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/filters/Coeffecients.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/filters/Filter.h>)

install (FILES Coeffecients.h Filter.h DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/filters"
		 COMPONENT limes_audio_dev)
