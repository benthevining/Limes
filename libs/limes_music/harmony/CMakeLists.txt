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

target_sources (
	limes_music
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchClass.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Pitch.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chord.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchClass.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Pitch.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chord.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchUtils.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/PitchClass.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/Pitch.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/Chord.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/PitchUtils.h>)

install (FILES PitchClass.h Pitch.h Chord.h PitchUtils.h
		 DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony" COMPONENT limes_music_dev)
