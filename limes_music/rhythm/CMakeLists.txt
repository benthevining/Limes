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
	limes_music
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteDuration.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteDuration_impl.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteLength.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteLength_impl.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteLength.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TimeSignature.h>
			$<INSTALL_INTERFACE:include/limes_music/NoteDuration.h>
			$<INSTALL_INTERFACE:include/limes_music/NoteDuration_impl.h>
			$<INSTALL_INTERFACE:include/limes_music/NoteLength.h>
			$<INSTALL_INTERFACE:include/limes_music/NoteLength_impl.h>
			$<INSTALL_INTERFACE:include/limes_music/TimeSignature.h>)

target_include_directories (limes_music PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
