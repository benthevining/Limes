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

add_subdirectory (limes_vecops)
add_subdirectory (limes_core)
add_subdirectory (limes_audio)
add_subdirectory (limes_music)

include (LimesConfigVersionHeader)

limes_config_version_header (
	FILENAME
	LimesVersion.h
	NAMESPACE
	limes
	FUNCTION_NAME
	getLimesVersion
	TARGET
	Limes
	SCOPE
	INTERFACE
	REL_PATH
	Limes/generated)

oranges_add_target_headers (
	TARGET
	Limes
	SCOPE
	INTERFACE
	REL_PATH
	Limes/generated
	FILES
	limes.h)

oranges_export_alias_target (Limes Limes)

oranges_install_targets (
	TARGETS
	Limes
	EXPORT
	LimesTargets
	REL_PATH
	Limes
	OPTIONAL)
