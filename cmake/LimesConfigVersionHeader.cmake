# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (OrangesCmakeDevTools)
include (GNUInstallDirs)

set (limes_header_input "${CMAKE_CURRENT_LIST_DIR}/scripts/VersionHeader.h" CACHE INTERNAL "")

#

function(limes_config_version_header)

	set (
		oneValueArgs
		NAMESPACE
		FUNCTION_NAME
		MAJOR
		MINOR
		PATCH
		FILENAME
		TARGET
		SCOPE
		REL_PATH
		INSTALL_COMPONENT)

	cmake_parse_arguments (LIMES_ARG "NO_INSTALL" "${oneValueArgs}" "" ${ARGN})

	lemons_require_function_arguments (LIMES_ARG NAMESPACE FUNCTION_NAME)
	lemons_check_for_unparsed_args (LIMES_ARG)
	oranges_assert_target_argument_is_target (LIMES_ARG)

	if(LIMES_ARG_NO_INSTALL AND LIMES_ARG_INSTALL_COMPONENT)
		message (
			WARNING
				"Arguments NO_INSTALL and INSTALL_COMPONENT cannot both be specified in call to ${CMAKE_CURRENT_FUNCTION}!"
			)
	endif()

	if(NOT LIMES_ARG_MAJOR)
		set (LIMES_ARG_MAJOR "${PROJECT_VERSION_MAJOR}")
	endif()

	if(NOT LIMES_ARG_MINOR)
		set (LIMES_ARG_MINOR "${PROJECT_VERSION_MINOR}")
	endif()

	if(NOT LIMES_ARG_PATCH)
		set (LIMES_ARG_PATCH "${PROJECT_VERSION_PATCH}")
	endif()

	if(NOT LIMES_ARG_FILENAME)
		if(LIMES_ARG_TARGET)
			set (LIMES_ARG_FILENAME "${LIMES_ARG_TARGET}Version.h")
		else()
			set (LIMES_ARG_FILENAME "${PROJECT_NAME}Version.h")
		endif()
	endif()

	if(NOT LIMES_ARG_REL_PATH)
		if(LIMES_ARG_TARGET)
			set (LIMES_ARG_REL_PATH "${LIMES_ARG_TARGET}/generated")
		else()
			set (LIMES_ARG_REL_PATH generated)
		endif()
	endif()

	set (output_dir "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${LIMES_ARG_REL_PATH}")
	set (output_header "${output_dir}/${LIMES_ARG_FILENAME}")

	configure_file ("${limes_header_input}" "${output_header}" @ONLY NEWLINE_STYLE UNIX)

	if(LIMES_ARG_TARGET)
		if(NOT TARGET ${LIMES_ARG_TARGET})
			message (
				FATAL_ERROR
					"${CMAKE_CURRENT_FUNCTION} called with non-existent target ${LIMES_ARG_TARGET}!"
				)
		endif()

		if(NOT LIMES_ARG_SCOPE)
			message (
				WARNING
					"You should specify the SCOPE argument to ${CMAKE_CURRENT_FUNCTION} to use the TARGET argument!"
				)
			set (LIMES_ARG_SCOPE INTERFACE)
		endif()

		target_sources (
			"${LIMES_ARG_TARGET}"
			"${LIMES_ARG_SCOPE}" $<BUILD_INTERFACE:${output_header}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${LIMES_ARG_REL_PATH}>)

		target_include_directories (
			"${LIMES_ARG_TARGET}" "${LIMES_ARG_SCOPE}" $<BUILD_INTERFACE:${output_dir}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${LIMES_ARG_REL_PATH}>)

		target_link_libraries ("${LIMES_ARG_TARGET}" "${LIMES_ARG_SCOPE}" Limes::limes_core)
	endif()

	if(NOT LIMES_ARG_NO_INSTALL)
		if(NOT LIMES_ARG_INSTALL_COMPONENT)
			if(LIMES_ARG_TARGET)
				set (LIMES_ARG_INSTALL_COMPONENT "${PROJECT_NAME}.${LIMES_ARG_TARGET}")
			else()
				set (LIMES_ARG_INSTALL_COMPONENT "${PROJECT_NAME}")
			endif()
		endif()

		install (FILES "${output_header}"
				 DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIMES_ARG_REL_PATH}"
				 COMPONENT "${LIMES_ARG_INSTALL_COMPONENT}")
	endif()

endfunction()

#

add_library (limesVersionHeaderTest STATIC EXCLUDE_FROM_ALL
			 "${CMAKE_CURRENT_LIST_DIR}/scripts/version_header_test.cpp")

target_link_libraries (limesVersionHeaderTest PRIVATE Limes::Limes)

add_test (NAME Limes.versionHeader COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
										   --target limesVersionHeaderTest)

set_tests_properties (
	Limes.versionHeader
	PROPERTIES
		PASS_REGULAR_EXPRESSION
		"major: ${PROJECT_VERSION_MAJOR}; minor: ${PROJECT_VERSION_MINOR}; patch: ${PROJECT_VERSION_PATCH}"
	)