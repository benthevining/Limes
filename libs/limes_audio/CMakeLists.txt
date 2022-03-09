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

add_library (limes_audio STATIC)

set_target_properties (limes_audio PROPERTIES VERSION "${PROJECT_VERSION}"
											  SOVERSION "${PROJECT_VERSION_MAJOR}")

target_link_libraries (limes_audio PRIVATE Limes::limes_core Oranges::OrangesDefaultTarget
										   Oranges::OrangesUnityBuild)

oranges_add_target_headers (
	TARGET
	limes_audio
	SCOPE
	PUBLIC
	REL_PATH
	Limes/limes_audio
	FILES
	limes_audio.h)

target_include_directories (
	limes_audio INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
						  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_audio>)

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/streams"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_audio/streams")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/util" "${CMAKE_CURRENT_BINARY_DIR}/limes_audio/util")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/psola" "${CMAKE_CURRENT_BINARY_DIR}/limes_audio/psola")

lemons_sort_target_sources (limes_audio "${CMAKE_CURRENT_LIST_DIR}")

oranges_export_alias_target (limes_audio Limes)

target_link_libraries (Limes INTERFACE Limes::limes_audio)

oranges_install_targets (
	TARGETS
	limes_audio
	EXPORT
	LimesTargets
	REL_PATH
	Limes/limes_audio
	COMPONENT_PREFIX
	limes_audio)
