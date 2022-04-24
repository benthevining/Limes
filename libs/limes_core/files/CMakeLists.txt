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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/FilesystemEntry.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/FilesystemEntry.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/file.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/file.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/directory.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/directory.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/sym_link.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/sym_link.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/files/FilesystemEntry.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/files/file.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/files/directory.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/files/sym_link.h>)

install (FILES FilesystemEntry.h file.h directory.h sym_link.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/files" COMPONENT limes_core_dev)
