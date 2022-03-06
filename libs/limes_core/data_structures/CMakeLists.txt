# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/vectors")

target_sources (
	limes_core
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Dimensions.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Dimensions.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedListModel.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedListModel_impl.h>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/Dimensions.h>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/Version.h>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/LinkedListModel.h>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/LinkedListModel_impl.h>
	)

target_include_directories (limes_core PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
