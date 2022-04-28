# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (intervals)
add_subdirectory (scales)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME harmony
	TARGET limes_music
	INSTALL_COMPONENT limes_music_dev
	INSTALL_DIR "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  Chord.cpp
		  Chord.h
		  Pitch.cpp
		  Pitch.h
		  PitchClass.cpp
		  PitchClass.h
		  PitchUtils.h)

set (harmony_files ${harmony_files} ${intervals_files} ${scales_files}
	 PARENT_SCOPE)
