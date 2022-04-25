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
		# cmake-format: sortable
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chromatic.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chromatic.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/KeySignature.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/KeySignature.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/KeySignature_impl.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Mode.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Mode.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Octatonic.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Octatonic.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Scale.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Scale.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/WholeTone.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/WholeTone.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Chromatic.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/KeySignature.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/KeySignature_impl.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Mode.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Octatonic.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/Scale.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales/WholeTone.h>)

install (
	FILES # cmake-format: sortable
		  Chromatic.h
		  KeySignature.h
		  KeySignature_impl.h
		  Mode.h
		  Octatonic.h
		  Scale.h
		  WholeTone.h
	DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/scales"
	COMPONENT limes_music_dev)

set (
	scales_files
	# cmake-format: sortable
	harmony/scales/Chromatic.cpp
	harmony/scales/Chromatic.h
	harmony/scales/KeySignature.cpp
	harmony/scales/KeySignature.h
	harmony/scales/KeySignature_impl.h
	harmony/scales/Mode.cpp
	harmony/scales/Mode.h
	harmony/scales/Octatonic.cpp
	harmony/scales/Octatonic.h
	harmony/scales/Scale.cpp
	harmony/scales/Scale.h
	harmony/scales/WholeTone.cpp
	harmony/scales/WholeTone.h
	PARENT_SCOPE)
