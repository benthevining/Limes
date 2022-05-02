# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME utilities
	TARGET limes_midi
	INSTALL_COMPONENT limes_midi_dev
	INSTALL_DIR "${LIMES_MIDI_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  PitchbendMapper.cpp PitchbendMapper.h VelocityMapper.cpp VelocityMapper.h)

set (utilities_files ${utilities_files} PARENT_SCOPE)
