# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesFunctionArgumentHelpers)
include (GNUInstallDirs)
include (OrangesDefaultTarget)

#

function (limes_add_binary_data_files)

	if (NOT TARGET Limes::BinaryBuilder)
		message (
			FATAL_ERROR
				"${CMAKE_CURRENT_FUNCTION} - target Limes::BinaryBuilder does not exist, cannot generate binary resource files!"
			)
	endif ()

	set (oneValueArgs OUTPUT_DIR BASE_NAME NAMESPACE CPP_OUTPUT_VAR H_OUTPUT_VAR)

	cmake_parse_arguments (LIMES_ARG "" "${oneValueArgs}" "INPUTS" ${ARGN})

	lemons_require_function_arguments (LIMES_ARG INPUTS)
	lemons_check_for_unparsed_args (LIMES_ARG)

	if (NOT LIMES_ARG_OUTPUT_DIR)
		set (LIMES_ARG_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/BinaryBuilderGenerated")
	endif ()

	if (NOT LIMES_ARG_BASE_NAME)
		set (LIMES_ARG_BASE_NAME BinaryData)
	endif ()

	if (NOT LIMES_ARG_NAMESPACE)
		set (LIMES_ARG_NAMESPACE BinaryData)
	endif ()

	# TO DO
	set (cpp_output "${LIMES_ARG_OUTPUT_DIR}/${LIMES_ARG_BASE_NAME}.cpp")
	set (h_output "${LIMES_ARG_OUTPUT_DIR}/${LIMES_ARG_BASE_NAME}.h")

	add_custom_command (
		OUTPUT "${cpp_output}" "${h_output}"
		COMMAND Limes::BinaryBuilder -d "${LIMES_ARG_OUTPUT_DIR}" -o "${LIMES_ARG_BASE_NAME}" -ns
				"${LIMES_ARG_NAMESPACE}" ${LIMES_ARG_INPUTS}
		COMMENT "Generating binary files for target ${LIMES_ARG_TARGET}..."
		VERBATIM USES_TERMINAL COMMAND_EXPAND_LISTS)

	if (LIMES_ARG_CPP_OUTPUT_VAR)
		set (${LIMES_ARG_CPP_OUTPUT_VAR} "${cpp_output}" PARENT_SCOPE)
	endif ()

	if (LIMES_ARG_H_OUTPUT_VAR)
		set (${LIMES_ARG_H_OUTPUT_VAR} "${h_output}" PARENT_SCOPE)
	endif ()

endfunction ()

#

function (limes_add_binary_data_target)

	set (oneValueArgs TARGET_NAME HEADER_NAME NAMESPACE INSTALL_REL_PATH INSTALL_COMPONENT)

	cmake_parse_arguments (LIMES_ARG "" "${oneValueArgs}" "INPUTS" ${ARGN})

	lemons_require_function_arguments (LIMES_ARG TARGET_NAME INPUTS)
	lemons_check_for_unparsed_args (LIMES_ARG)

	if (TARGET "${LIMES_ARG_TARGET_NAME}")
		message (
			FATAL_ERROR
				"${CMAKE_CURRENT_FUNCTION} - target ${LIMES_ARG_TARGET_NAME} already exists!")
	endif ()

	add_library ("${LIMES_ARG_TARGET_NAME}" STATIC)

	target_link_libraries ("${LIMES_ARG_TARGET_NAME}" PRIVATE Oranges::OrangesDefaultTarget)

	if (NOT LIMES_ARG_HEADER_NAME)
		set (LIMES_ARG_HEADER_NAME BinaryData)
	endif ()

	if (NOT LIMES_ARG_NAMESPACE)
		set (LIMES_ARG_NAMESPACE BinaryData)
	endif ()

	set (output_dir "${CMAKE_CURRENT_BINARY_DIR}/${LIMES_ARG_TARGET_NAME}")

	limes_add_binary_data_files (
		BASE_NAME "${LIMES_ARG_HEADER_NAME}"
		NAMESPACE "${LIMES_ARG_NAMESPACE}"
		OUTPUT_DIR "${output_dir}"
		INPUTS ${LIMES_ARG_INPUTS}
		CPP_OUTPUT_VAR cpp_file
		H_OUTPUT_VAR h_file)

	if (LIMES_ARG_INSTALL_REL_PATH)
		set (install_dir "${CMAKE_INSTALL_INCLUDEDIR}/${LIMES_ARG_INSTALL_REL_PATH}")
	else ()
		set (install_dir "${CMAKE_INSTALL_INCLUDEDIR}")
	endif ()

	target_sources (
		"${LIMES_ARG_TARGET_NAME}" PRIVATE "$<BUILD_INTERFACE:${cpp_file}>"
		PUBLIC "$<BUILD_INTERFACE:${h_file}>"
			   "$<INSTALL_INTERFACE:${install_dir}/${LIMES_ARG_HEADER_NAME}.h>")

	target_include_directories (
		"${LIMES_ARG_TARGET_NAME}" INTERFACE "$<BUILD_INTERFACE:${output_dir}>"
											 "$<INSTALL_INTERFACE:${install_dir}>")

	if (NOT LIMES_ARG_INSTALL_COMPONENT)
		set (LIMES_ARG_INSTALL_COMPONENT binary_data)
	endif ()

	install (FILES "${h_file}" DESTINATION "${install_dir}"
			 COMPONENT "${LIMES_ARG_INSTALL_COMPONENT}")

endfunction ()
