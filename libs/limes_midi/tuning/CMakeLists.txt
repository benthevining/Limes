# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

find_package (MTS-ESP REQUIRED COMPONENTS Client)

target_link_libraries (limes_midi PUBLIC ODDSound::MTSClient)

target_sources (
	limes_midi
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/mts_client.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/mts_client.h>
			$<INSTALL_INTERFACE:${LIMES_MIDI_INSTALL_INCLUDE_DIR}/tuning/mts_client.h>)

install (FILES mts_client.h DESTINATION "${LIMES_MIDI_INSTALL_INCLUDE_DIR}/tuning"
		 COMPONENT limes_midi_dev)
