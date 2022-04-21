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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ReferenceCountedObject.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ReferenceCountedObject.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/SharedObject.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StaticObjects.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/alignment.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/ReferenceCountedObject.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/SharedObject.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/StaticObjects.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/alignment.h>)

install (FILES ReferenceCountedObject.h SharedObject.h StaticObjects.h alignment.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory" COMPONENT limes_core_dev)
