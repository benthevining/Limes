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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteLength.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteDuration.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteDuration_impl.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteLength.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NoteLength_impl.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TimeSignature.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/rhythm/NoteDuration.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/rhythm/NoteDuration_impl.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/rhythm/NoteLength.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/rhythm/NoteLength_impl.h>
			$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/rhythm/TimeSignature.h>)

install (FILES NoteDuration.h NoteDuration_impl.h NoteLength.h NoteLength_impl.h TimeSignature.h
		 DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/rhythm" COMPONENT limes_music_dev)
