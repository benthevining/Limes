# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (vectors)

target_sources (
	limes_core
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Dimensions.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Dimensions.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedListModel.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedListModel_impl.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/Dimensions.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/Version.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/LinkedListModel.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/LinkedListModel_impl.h>)

install (
	FILES "${CMAKE_CURRENT_LIST_DIR}/Dimensions.h" "${CMAKE_CURRENT_LIST_DIR}/Version.h"
		  "${CMAKE_CURRENT_LIST_DIR}/LinkedListModel.h"
		  "${CMAKE_CURRENT_LIST_DIR}/LinkedListModel_impl.h"
	DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	COMPONENT limes_core_dev)
