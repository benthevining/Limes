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
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chord.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chord.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Pitch.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Pitch.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchClass.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchClass.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchUtils.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/Chord.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/Pitch.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/PitchClass.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/PitchUtils.h>)

install (FILES # cmake-format: sortable
			   Chord.h Pitch.h PitchClass.h PitchUtils.h
		 DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony" COMPONENT limes_music_dev)

set (
	harmony_files
	# cmake-format: sortable
	${intervals_files}
	${scales_files}
	harmony/Chord.cpp
	harmony/Chord.h
	harmony/Pitch.cpp
	harmony/Pitch.h
	harmony/PitchClass.cpp
	harmony/PitchClass.h
	harmony/PitchUtils.h
	PARENT_SCOPE)
