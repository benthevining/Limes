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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StringUtils.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StringUtils.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TextTable.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TextTable.cpp>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/StringUtils.h>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/TextTable.h>)

target_include_directories (limes_core PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
