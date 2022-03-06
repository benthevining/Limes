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

find_package (Doxygen REQUIRED dot)

if(NOT TARGET Doxygen::doxygen)
	message (WARNING "Doxygen dependencies missing!")
	return ()
endif()

add_custom_target (
	LimesDoxygen
	COMMAND Doxygen::doxygen
	VERBATIM
	DEPENDS "${PROJECT_SOURCE_DIR}/Doxyfile"
	BYPRODUCTS "doc/index.html"
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
	COMMENT "Running Doxygen...")

add_custom_command (TARGET LimesDoxygen PRE_BUILD COMMAND Doxygen::doxygen --version
					COMMENT "Doxygen version:")

set_target_properties (
	LimesDoxygen PROPERTIES ADDITIONAL_CLEAN_FILES
							"${PROJECT_SOURCE_DIR}/doc;${PROJECT_SOURCE_DIR}/man")

install (DIRECTORY "${PROJECT_SOURCE_DIR}/man/man3" TYPE MAN COMPONENT Limes_Documentation
		 EXCLUDE_FROM_ALL OPTIONAL)

install (DIRECTORY "${PROJECT_SOURCE_DIR}/doc" TYPE INFO COMPONENT Limes_Documentation
		 EXCLUDE_FROM_ALL OPTIONAL)
