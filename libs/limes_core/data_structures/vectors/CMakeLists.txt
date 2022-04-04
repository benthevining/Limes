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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/vector.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/basic_vector.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/scalar_vector.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/owned_vector.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/vector.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/basic_vector.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/scalar_vector.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/owned_vector.h>)

install (
	FILES "${CMAKE_CURRENT_LIST_DIR}/vector.h" "${CMAKE_CURRENT_LIST_DIR}/basic_vector.h"
		  "${CMAKE_CURRENT_LIST_DIR}/scalar_vector.h" "${CMAKE_CURRENT_LIST_DIR}/owned_vector.h"
	DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}" COMPONENT limes_core_dev)
