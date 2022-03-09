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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Scale.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Chromatic.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/KeySignature.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Mode.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Octatonic.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/WholeTone.cpp>)

oranges_add_target_headers (
	TARGET
	limes_music
	SCOPE
	PRIVATE
	REL_PATH
	Limes/limes_music
	FILES
	Scale.h
	Chromatic.h
	KeySignature.h
	KeySignature_impl.h
	Mode.h
	Octatonic.h
	WholeTone.h)
