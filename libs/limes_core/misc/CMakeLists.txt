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
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Functions.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/IgnoreUnused.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/Version.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/Algorithm.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/TypeTraits.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/Functions.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/IgnoreUnused.h>)

install (FILES Algorithm.h TypeTraits.h Version.h Functions.h IgnoreUnused.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc" COMPONENT limes_core_dev)
