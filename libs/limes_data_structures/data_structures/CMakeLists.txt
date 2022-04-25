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
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Dimensions.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Dimensions.h>
		$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/data_structures/Dimensions.h>
	)

install (FILES Dimensions.h
		 DESTINATION "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/data_structures"
		 COMPONENT limes_data_structures_dev)

set (data_structures_files data_structures/Dimensions.cpp data_structures/Dimensions.h PARENT_SCOPE)
