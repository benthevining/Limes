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
	limes_core
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StringUtils.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TextTable.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StringUtils.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TextTable.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/text/StringUtils.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/text/TextTable.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/StringUtils.h" "${CMAKE_CURRENT_LIST_DIR}/TextTable.h"
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/text" COMPONENT limes_core_dev)
