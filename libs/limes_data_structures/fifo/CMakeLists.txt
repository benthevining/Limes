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
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/AbstractFIFO.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/AbstractFIFO.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/FIFO.h>
			$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/fifo/AbstractFIFO.h>
			$<INSTALL_INTERFACE:${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/fifo/FIFO.h>)

install (FILES # cmake-format: sortable
			   AbstractFIFO.h FIFO.h DESTINATION "${LIMES_DATA_STRUCTURES_INSTALL_INCLUDE_DIR}/fifo"
		 COMPONENT limes_data_structures_dev)

set (fifo_files # cmake-format: sortable
				fifo/AbstractFIFO.cpp fifo/AbstractFIFO.h fifo/FIFO.h PARENT_SCOPE)
