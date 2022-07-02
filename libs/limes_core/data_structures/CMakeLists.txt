# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (dimensions)
add_subdirectory (fifo)
add_subdirectory (linked_list)
add_subdirectory (version)

set (data_structures_files # cmake-format: sortable
						   ${dimensions_files} ${fifo_files} ${linked_list_files} ${version_files}
	 PARENT_SCOPE)
