# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_library (Limes INTERFACE)

set_target_properties (Limes PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION
																	 "${PROJECT_VERSION_MAJOR}")

include (LemonsCmakeDevTools)

add_subdirectory (limes_vecops)
add_subdirectory (limes_core)
add_subdirectory (limes_audio)
add_subdirectory (limes_music)

target_sources (Limes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes.h>
								$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/Limes.h>)

target_include_directories (Limes INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
											$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes>)

oranges_export_alias_target (Limes Limes)

oranges_install_targets (
	TARGETS
	Limes
	EXPORT
	LimesTargets
	REL_PATH
	Limes
	OPTIONAL
	REL_PATH
	Limes)
