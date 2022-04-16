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
	limes_data_structures
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/vector.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/basic_vector.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/scalar_vector.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/owned_vector.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/vectors/vector.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/vectors/basic_vector.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/vectors/scalar_vector.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/vectors/owned_vector.h>)

install (
	FILES "${CMAKE_CURRENT_LIST_DIR}/vector.h" "${CMAKE_CURRENT_LIST_DIR}/basic_vector.h"
		  "${CMAKE_CURRENT_LIST_DIR}/scalar_vector.h" "${CMAKE_CURRENT_LIST_DIR}/owned_vector.h"
	DESTINATION "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/vectors"
	COMPONENT limes_data_structures_dev)
