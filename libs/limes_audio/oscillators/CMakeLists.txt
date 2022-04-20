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
	limes_audio
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/basic_oscillators.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/basic_oscillators.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/DetunableModel.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/DetunableModel.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/oscillators/basic_oscillators.h>
			$<INSTALL_INTERFACE:${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/oscillators/DetunableModel.h>)

install (FILES basic_oscillators.h DetunableModel.h
		 DESTINATION "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}/oscillators" COMPONENT limes_audio_dev)
