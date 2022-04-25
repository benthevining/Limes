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
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchbendMapper.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchbendMapper.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/VelocityMapper.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/VelocityMapper.h>
			$<INSTALL_INTERFACE:${LIMES_MIDI_INSTALL_INCLUDE_DIR}/utilities/PitchbendMapper.h>
			$<INSTALL_INTERFACE:${LIMES_MIDI_INSTALL_INCLUDE_DIR}/utilities/VelocityMapper.h>)

install (FILES # cmake-format: sortable
			   PitchbendMapper.h VelocityMapper.h
		 DESTINATION "${LIMES_MIDI_INSTALL_INCLUDE_DIR}/utilities" COMPONENT limes_midi_dev)

set (
	utilities_files
	# cmake-format: sortable
	utilities/PitchbendMapper.cpp utilities/PitchbendMapper.h utilities/VelocityMapper.cpp
	utilities/VelocityMapper.h
	PARENT_SCOPE)
