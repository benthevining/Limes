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
		# cmake-format: sortable
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedList.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedList_impl.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedListModel.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/LinkedListModel_impl.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/linked_list/LinkedList.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/linked_list/LinkedList_impl.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/linked_list/LinkedListModel.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/linked_list/LinkedListModel_impl.h>
	)

install (
	FILES # cmake-format: sortable
		  LinkedList.h LinkedList_impl.h LinkedListModel.h LinkedListModel_impl.h
	DESTINATION "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/linked_list"
	COMPONENT limes_data_structures_dev)

set (
	linked_list_files
	# cmake-format: sortable
	linked_list/LinkedList.h linked_list/LinkedList_impl.h linked_list/LinkedListModel.h
	linked_list/LinkedListModel_impl.h
	PARENT_SCOPE)
