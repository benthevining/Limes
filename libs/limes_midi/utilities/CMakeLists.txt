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
	limes_midi
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/VelocityMapper.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/VelocityMapper.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchbendMapper.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchbendMapper.h>
			$<INSTALL_INTERFACE:${LIMES_MIDI_INSTALL_INCLUDE_DIR}/utilities/VelocityMapper.h>
			$<INSTALL_INTERFACE:${LIMES_MIDI_INSTALL_INCLUDE_DIR}/utilities/PitchbendMapper.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/VelocityMapper.h"
			   "${CMAKE_CURRENT_LIST_DIR}/PitchbendMapper.h"
		 DESTINATION "${LIMES_MIDI_INSTALL_INCLUDE_DIR}/utilities" COMPONENT limes_midi_dev)
