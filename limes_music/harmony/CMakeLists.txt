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
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchClass.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Pitch.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Pitch.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chord.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chord.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PitchUtils.h>
			$<INSTALL_INTERFACE:include/PitchClass.h>
			$<INSTALL_INTERFACE:include/Pitch.h>
			$<INSTALL_INTERFACE:include/Chord.h>
			$<INSTALL_INTERFACE:include/PitchUtils.h>)

target_include_directories (limes_core PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
