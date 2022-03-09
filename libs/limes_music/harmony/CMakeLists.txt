# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/intervals")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/scales")

target_sources (
	limes_music
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchClass.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Pitch.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chord.cpp>)

oranges_add_target_headers (
	TARGET
	limes_music
	SCOPE
	PRIVATE
	REL_PATH
	Limes/limes_music
	FILES
	PitchClass.h
	Pitch.h
	Chord.h
	PitchUtils.h)
