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

execute_process (COMMAND "@DOXYGEN_EXECUTABLE@" --version OUTPUT_VARIABLE doxygen_version
				 OUTPUT_STRIP_TRAILING_WHITESPACE)

message (STATUS "Doxygen version: ${doxygen_version}")

file (REMOVE "@LIMES_LOG_FILE@")

file (MAKE_DIRECTORY "@docs_output@")

execute_process (COMMAND "@DOXYGEN_EXECUTABLE@" "@configured_doxyfile@"
				 TIMEOUT 600 COMMAND_ERROR_IS_FATAL ANY)
