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
	limes_vecops PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fft.h>
						$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_vecops/fft.h>
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fft.cpp>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/fft.h"
		 DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_vecops"
		 COMPONENT limes_vecops_Development)
