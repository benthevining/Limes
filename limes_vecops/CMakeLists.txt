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

add_library (limes_vecops INTERFACE)

target_link_libraries (limes_vecops INTERFACE Oranges::OrangesDefaultTarget
											  Oranges::OrangesUnityBuild)

target_sources (
	limes_vecops
	INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h>
			  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/util/Platform.h>
			  $<INSTALL_INTERFACE:install/limes_vecops.h> $<INSTALL_INTERFACE:install/Platform.h>)

target_include_directories (limes_vecops INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
												   $<INSTALL_INTERFACE:include>)

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/impl")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/util")

lemons_sort_target_sources (limes_vecops ${CMAKE_CURRENT_LIST_DIR})

set_target_properties (limes_vecops PROPERTIES LINKER_LANGUAGE CXX)

add_library (Limes::limes_vecops ALIAS limes_vecops)

target_link_libraries (Limes INTERFACE Limes::limes_vecops)

oranges_install_targets (TARGETS limes_vecops EXPORT LimesTargets)

get_target_property (vecops_sources limes_vecops INTERFACE_SOURCES)

install (FILES ${vecops_sources} TYPE INCLUDE)
