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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Algorithm.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TypeTraits.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StaticObjects.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Functions.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/IgnoreUnused.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/Version.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/Algorithm.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/TypeTraits.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/StaticObjects.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/Functions.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/IgnoreUnused.h>)

install (
	FILES "${CMAKE_CURRENT_LIST_DIR}/Algorithm.h" "${CMAKE_CURRENT_LIST_DIR}/TypeTraits.h"
		  "${CMAKE_CURRENT_LIST_DIR}/Version.h" "${CMAKE_CURRENT_LIST_DIR}/StaticObjects.h"
		  "${CMAKE_CURRENT_LIST_DIR}/Functions.h" "${CMAKE_CURRENT_LIST_DIR}/IgnoreUnused.h"
	DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	COMPONENT limes_core_dev)
