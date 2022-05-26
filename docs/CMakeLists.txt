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
	VERSION "${Limes_VERSION}"
	LANGUAGES NONE
	DESCRIPTION "Limes documentation"
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}")

include (OrangesGraphviz)

oranges_add_graphviz_target (TARGET LimesDependencyGraph SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/.."
							 COPY_GRAPH_TO "${CMAKE_CURRENT_LIST_DIR}/../util/deps_graph.png")

find_package (Doxygen COMPONENTS dot)

if (NOT TARGET Doxygen::doxygen)
	message (WARNING "Doxygen dependencies missing!")
	return ()
endif ()

include (CPackComponent)
include (GNUInstallDirs)

set (docs_output "${CMAKE_CURRENT_LIST_DIR}/../doc")

set_property (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" APPEND PROPERTY ADDITIONAL_CLEAN_FILES
																	"${docs_output}")

if (Limes_IS_TOP_LEVEL)
	set (all_flag ALL)
endif ()

add_custom_target (
	LimesDocs
	${all_flag}
	COMMAND Doxygen::doxygen --version
	COMMAND Doxygen::doxygen "${CMAKE_CURRENT_LIST_DIR}/Doxyfile"
	WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
	VERBATIM USES_TERMINAL
	BYPRODUCTS "${docs_output}"
	COMMENT "Building Limes documentation...")

unset (all_flag)

add_dependencies (LimesDocs LimesDependencyGraph)

set_target_properties (LimesDocs PROPERTIES FOLDER Utility LABELS "Limes;Utility"
											XCODE_GENERATE_SCHEME OFF)

#[[
Required for Latex builds:

sudo tlmgr install collection-latexextra wasysym helvetic wasy courier
]]

find_program (LATEX_EXECUTABLE NAMES pdflatex DOC "pdflatex tool")

find_program (MAKE_EXECUTABLE NAMES make nmake gmake
			  DOC "make executable used for building latex PDF")

if (LATEX_EXECUTABLE AND MAKE_EXECUTABLE AND OFF)
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

	cpack_add_component (limes_doc_pdf DISPLAY_NAME "Limes PDF documentation"
						 DESCRIPTION "Install the Limes PDF documentation" GROUP limes_docs)
endif ()

install (DIRECTORY "${docs_output}/html" COMPONENT limes_doc_html
		 DESTINATION "${CMAKE_INSTALL_DOCDIR}")

cpack_add_component (limes_doc_html DISPLAY_NAME "Limes HTML docs"
					 DESCRIPTION "Install the Limes HTML documentation" GROUP limes_docs)

install (DIRECTORY "${docs_output}/man3" DESTINATION "${CMAKE_INSTALL_MANDIR}"
		 COMPONENT limes_doc_man)

cpack_add_component (limes_doc_man DISPLAY_NAME "Limes man pages"
					 DESCRIPTION "Install the Limes man pages" GROUP limes_docs)

# install (FILES "${docs_output}/Limes.tag" TYPE INFO COMPONENT limes_docs)

cpack_add_component_group (
	limes_docs DISPLAY_NAME "Limes documentation" DESCRIPTION "Install all Limes documentation"
	PARENT_GROUP limes INSTALL_TYPES Developer)
