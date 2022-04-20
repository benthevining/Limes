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
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Scale.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chromatic.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/KeySignature.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Mode.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Octatonic.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/WholeTone.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Scale.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chromatic.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/KeySignature.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/KeySignature_impl.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Mode.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Octatonic.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/WholeTone.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Scale.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Chromatic.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/KeySignature.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/KeySignature_impl.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Mode.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Octatonic.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/WholeTone.h>)

install (
	FILES Scale.h
		  Chromatic.h
		  KeySignature.h
		  KeySignature_impl.h
		  Mode.h
		  Octatonic.h
		  WholeTone.h
	DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales"
	COMPONENT limes_music_dev)
