# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME linked_list
	TARGET limes_data_structures
	INSTALL_COMPONENT limes_data_structures_dev
	INSTALL_DIR "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  LinkedList.h LinkedList_impl.h LinkedListModel.h LinkedListModel_impl.h)

set (linked_list_files ${linked_list_files} PARENT_SCOPE)
