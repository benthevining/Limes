# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (RealtimeSafeObject)

target_sources (
	limes_core
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/RealtimeSafeObject.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/RealtimeTypeTraits.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Threading.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Threading.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ThreadSafeObject.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeSafeObject.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeTypeTraits.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/Threading.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/ThreadSafeObject.h>)

install (FILES # cmake-format: sortable
			   RealtimeSafeObject.h RealtimeTypeTraits.h Threading.h ThreadSafeObject.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading" COMPONENT limes_core_dev)

set (
	threading_files
	# cmake-format: sortable
	${rtso_files} threading/RealtimeSafeObject.h threading/RealtimeTypeTraits.h
	threading/Threading.cpp threading/Threading.h threading/ThreadSafeObject.h
	PARENT_SCOPE)
