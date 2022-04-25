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
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StringUtils.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StringUtils.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TextTable.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TextTable.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/text/StringUtils.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/text/TextTable.h>)

install (FILES # cmake-format: sortable
			   StringUtils.h TextTable.h DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/text"
		 COMPONENT limes_core_dev)

set (text_files # cmake-format: sortable
				text/StringUtils.cpp text/StringUtils.h text/TextTable.cpp text/TextTable.h
	 PARENT_SCOPE)
