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

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

if (ORANGES_IN_GRAPHVIZ_CONFIG)
	return ()
endif ()

project (
	LimesDocs
	VERSION 4.8.1
	LANGUAGES NONE
	DESCRIPTION "Limes documentation"
	HOMEPAGE_URL "www.github.com/benthevining/Limes")

if (PROJECT_IS_TOP_LEVEL)
	find_package (Oranges 2.24.0 REQUIRED)

	# need to enable at least one language for GNUInstallDirs
	enable_language (CXX)

	set (Limes_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
endif ()

option (LIMES_INTERNAL_DOCS
		"Display documentation meant for internal usage in development of Limes" OFF)

include (OrangesGraphviz)

oranges_add_graphviz_target (TARGET LimesDependencyGraph SOURCE_DIR "${Limes_SOURCE_DIR}"
							 COPY_GRAPH_TO "${Limes_SOURCE_DIR}/util/deps_graph.png")

#

find_package (Doxygen COMPONENTS dot)

if (NOT Doxygen_FOUND)
	message (WARNING "Doxygen dependencies missing!")
	return ()
endif ()

include (CPackComponent)
include (GNUInstallDirs)

set (docs_output "${Limes_SOURCE_DIR}/doc")

set_property (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" APPEND PROPERTY ADDITIONAL_CLEAN_FILES
																	"${docs_output}")

if (LIMES_INTERNAL_DOCS)
	set (LIMES_INTERNAL_DOCS YES)
	set (LIMES_ENABLED_SECTIONS internals)
else ()
	set (LIMES_INTERNAL_DOCS NO)
	set (LIMES_ENABLED_SECTIONS "")
endif ()

set (LIMES_LOG_FILE "${docs_output}/doxygen.log")

set (configured_doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

configure_file ("${CMAKE_CURRENT_LIST_DIR}/Doxyfile" "${configured_doxyfile}" @ONLY
				NEWLINE_STYLE UNIX)

unset (LIMES_INTERNAL_DOCS)
unset (LIMES_ENABLED_SECTIONS)

set (configured_script "${CMAKE_CURRENT_BINARY_DIR}/build_docs.cmake")

configure_file ("${CMAKE_CURRENT_LIST_DIR}/build_docs.cmake" "${configured_script}" @ONLY
				NEWLINE_STYLE UNIX)

if (Limes_IS_TOP_LEVEL OR PROJECT_IS_TOP_LEVEL)
	set (all_flag ALL)
endif ()

add_custom_target (
	LimesDocs
	${all_flag}
	COMMAND "${CMAKE_COMMAND}" -P "${configured_script}"
	VERBATIM USES_TERMINAL
	BYPRODUCTS "${docs_output}"
	COMMENT "Building Limes Doxygen documentation..."
	SOURCES Doxyfile DoxygenLayout.xml "${configured_script}")

unset (LIMES_LOG_FILE)
unset (all_flag)
unset (configured_doxyfile)
unset (configured_script)

add_dependencies (LimesDocs LimesDependencyGraph)

set_target_properties (LimesDocs PROPERTIES FOLDER Utility LABELS "Limes;Utility;Documentation"
											XCODE_GENERATE_SCHEME OFF)

#[[
Required for Latex builds:

sudo tlmgr install collection-latexextra wasysym helvetic wasy courier
brew install gs
]]

find_package (LATEX COMPONENTS BIBTEX PDFLATEX)

if (LATEX_FOUND AND OFF)
	find_program (MAKE_EXECUTABLE NAMES make nmake gmake
				  DOC "make executable used for building latex PDF")

	if (MAKE_EXECUTABLE)
		add_custom_command (
			TARGET LimesDocs
			POST_BUILD
			COMMAND "${MAKE_EXECUTABLE}"
			COMMAND "${CMAKE_COMMAND}" -E copy "${docs_output}/latex/refman.pdf"
					"${docs_output}/Limes.pdf"
			WORKING_DIRECTORY "${docs_output}/latex"
			COMMENT "Building PDF from Latex..."
			VERBATIM USES_TERMINAL)

		install (FILES "${docs_output}/Limes.pdf" DESTINATION "${CMAKE_INSTALL_DOCDIR}"
				 COMPONENT limes_doc_pdf)

		cpack_add_component (
			limes_doc_pdf DISPLAY_NAME "Limes PDF documentation"
			DESCRIPTION "Install the Limes Doxygen PDF documentation" GROUP limes_docs)
	endif ()
endif ()

install (DIRECTORY "${docs_output}/html" COMPONENT limes_doc_html
		 DESTINATION "${CMAKE_INSTALL_DOCDIR}")

cpack_add_component (limes_doc_html DISPLAY_NAME "Limes HTML docs"
					 DESCRIPTION "Install the Limes Doxygen HTML documentation" GROUP limes_docs)

install (DIRECTORY "${docs_output}/man3" DESTINATION "${CMAKE_INSTALL_MANDIR}"
		 COMPONENT limes_doc_man)

cpack_add_component (limes_doc_man DISPLAY_NAME "Limes man pages"
					 DESCRIPTION "Install the Limes Doxygen man pages" GROUP limes_docs)

# install (FILES "${docs_output}/Limes.tag" TYPE INFO COMPONENT limes_docs)

cpack_add_component_group (
	limes_docs DISPLAY_NAME "All Limes documentation" DESCRIPTION "Install all Limes documentation"
	PARENT_GROUP limes INSTALL_TYPES Developer)
