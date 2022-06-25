# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

if (NOT EXISTS "@configured_doxyfile@")
	message (FATAL_ERROR "Configured Doxyfile does not exist at path '@configured_doxyfile@'")
endif ()

execute_process (COMMAND "@DOXYGEN_EXECUTABLE@" --version OUTPUT_VARIABLE doxygen_version
				 OUTPUT_STRIP_TRAILING_WHITESPACE)

message (STATUS "Doxygen version: ${doxygen_version}")

file (REMOVE "@LIMES_LOG_FILE@")

file (MAKE_DIRECTORY "@docs_output@")

execute_process (COMMAND "@DOXYGEN_EXECUTABLE@" "@configured_doxyfile@"
				 TIMEOUT 600 COMMAND_ERROR_IS_FATAL ANY)

if ("@MAKE_EXECUTABLE@" STREQUAL "" OR NOT "@LATEX_FOUND@")
	return ()
endif ()

message (STATUS "Building Latex PDF...")

execute_process (
	COMMAND "@MAKE_EXECUTABLE@"
	WORKING_DIRECTORY "@latex_output_dir@" # latex_output_dir
	TIMEOUT 600
	OUTPUT_VARIABLE latex_output
	ERROR_VARIABLE latex_error)

if (latex_error)
	set (latex_error_text "Errors:\n\n${latex_error}")
endif ()

file (WRITE "@docs_output@/latex.log" ${latex_error_text} "\n\n\nOutput:\n\n${latex_output}")

set (generated_pdf "@latex_output_dir@/refman.pdf")

if (EXISTS "${generated_pdf}")
	# cmake-lint: disable=E1126
	file (COPY_FILE "${generated_pdf}" "@docs_output@/Limes.pdf")

	message (STATUS "PDF built successfully!")
endif ()
