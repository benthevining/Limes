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
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Algorithm.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Functions.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/IgnoreUnused.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/preprocessor.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/streams.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/TypeTraits.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Version.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/Algorithm.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/Functions.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/IgnoreUnused.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/preprocessor.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/streams.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/TypeTraits.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc/Version.h>)

install (FILES # cmake-format: sortable
			   Algorithm.h Functions.h IgnoreUnused.h preprocessor.h TypeTraits.h Version.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/misc" COMPONENT limes_core_dev)

set (
	misc_files
	# cmake-format: sortable
	misc/Algorithm.h
	misc/Functions.h
	misc/IgnoreUnused.h
	misc/preprocessor.h
	misc/streams.h
	misc/TypeTraits.h
	misc/Version.cpp
	misc/Version.h
	PARENT_SCOPE)
