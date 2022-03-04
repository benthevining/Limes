# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (LemonsDefaultProjectSettings)
include (OrangesDefaultTarget)

add_library (limes_vecops INTERFACE)

target_link_libraries (limes_vecops INTERFACE Oranges::OrangesDefaultTarget)

target_sources (
	limes_vecops
	INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h>
			  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/util/Platform.h>
			  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/util/Constants.h>
			  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/impl/fallback_impl.h>)

target_include_directories (limes_vecops INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
												   $<INSTALL_INTERFACE:include>)

include ("${CMAKE_CURRENT_LIST_DIR}/util/ConfigVecops.cmake")

lemons_sort_target_sources (limes_vecops ${CMAKE_CURRENT_LIST_DIR})

set_target_properties (limes_vecops PROPERTIES LINKER_LANGUAGE CXX)

add_library (Limes::limes_vecops ALIAS limes_vecops)

target_link_libraries (Limes INTERFACE limes_vecops)

install (TARGETS limes_vecops EXPORT LimesTargets)
