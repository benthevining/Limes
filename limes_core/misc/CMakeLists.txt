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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Algorithm.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TypeTraits.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/languages.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/languages.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StaticObjects.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Functions.h>
			$<INSTALL_INTERFACE:include/Algorithm.h>
			$<INSTALL_INTERFACE:include/TypeTraits.h>
			$<INSTALL_INTERFACE:include/languages.h>
			$<INSTALL_INTERFACE:include/StaticObjects.h>
			$<INSTALL_INTERFACE:include/Functions.h>)

target_include_directories (limes_core PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
