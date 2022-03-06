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

add_library (limes_music STATIC)

target_link_libraries (limes_music PRIVATE Oranges::OrangesDefaultTarget Oranges::OrangesUnityBuild
										   Limes::limes_core)

target_sources (limes_music PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_music.h>
								   $<INSTALL_INTERFACE:include/limes_music/limes_music.h>)

target_include_directories (limes_music INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
												  $<INSTALL_INTERFACE:include/limes_music>)

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/harmony"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_music/harmony")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/rhythm"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_music/rhythm")

lemons_sort_target_sources (limes_music ${CMAKE_CURRENT_LIST_DIR})

oranges_export_alias_target (limes_music Limes)

target_link_libraries (Limes INTERFACE Limes::limes_music)

oranges_install_targets (TARGETS limes_music EXPORT LimesTargets REL_PATH "${LIMES_INSTALL_DEST}")
