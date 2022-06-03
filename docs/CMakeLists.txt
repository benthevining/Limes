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
	HOMEPAGE_URL "${Limes_HOMEPAGE_URL}")

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

# Doxygen

find_package (Doxygen COMPONENTS dot)

if (NOT TARGET Doxygen::doxygen)
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

if (Limes_IS_TOP_LEVEL)
	set (all_flag ALL)
endif ()

add_custom_target (
	LimesDoxygen
	${all_flag}
	COMMAND Doxygen::doxygen --version
	COMMAND "${CMAKE_COMMAND}" -E rm -f "${LIMES_LOG_FILE}"
	COMMAND "${CMAKE_COMMAND}" -E make_directory "${docs_output}"
	COMMAND Doxygen::doxygen "${configured_doxyfile}"
	VERBATIM USES_TERMINAL
	BYPRODUCTS "${docs_output}"
	COMMENT "Building Limes Doxygen documentation..."
	SOURCES Doxyfile DoxygenLayout.xml)

unset (LIMES_LOG_FILE)
unset (all_flag)
unset (configured_doxyfile)

add_dependencies (LimesDoxygen LimesDependencyGraph)

set_target_properties (LimesDoxygen PROPERTIES FOLDER Utility LABELS "Limes;Utility;Documentation"
											   XCODE_GENERATE_SCHEME OFF)

#[[
Required for Latex builds:

sudo tlmgr install collection-latexextra wasysym helvetic wasy courier
brew install gs
]]

find_program (LATEX_EXECUTABLE NAMES pdflatex
			  DOC "pdflatex tool. Not actually used, just checked for existence.")

find_program (MAKE_EXECUTABLE NAMES make nmake gmake
			  DOC "make executable used for building latex PDF")

if (LATEX_EXECUTABLE AND MAKE_EXECUTABLE AND OFF)
	add_custom_command (
		TARGET LimesDoxygen
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
		limes_doc_doxy_pdf DISPLAY_NAME "Limes PDF documentation"
		DESCRIPTION "Install the Limes Doxygen PDF documentation" GROUP limes_doxy_docs)
endif ()

install (DIRECTORY "${docs_output}/html" COMPONENT limes_doc_doxy_html
		 DESTINATION "${CMAKE_INSTALL_DOCDIR}")

cpack_add_component (
	limes_doc_doxy_html DISPLAY_NAME "Limes HTML docs"
	DESCRIPTION "Install the Limes Doxygen HTML documentation" GROUP limes_doxy_docs)

install (DIRECTORY "${docs_output}/man3" DESTINATION "${CMAKE_INSTALL_MANDIR}"
		 COMPONENT limes_doc_doxy_man)

cpack_add_component (limes_doc_man DISPLAY_NAME "Limes man pages"
					 DESCRIPTION "Install the Limes Doxygen man pages" GROUP limes_doxy_docs)

# install (FILES "${docs_output}/Limes.tag" TYPE INFO COMPONENT limes_doxy_docs)

cpack_add_component_group (
	limes_doxy_docs DISPLAY_NAME "All Limes Doxygen documentation"
	DESCRIPTION "Install all Limes Doxygen documentation" PARENT_GROUP limes_docs INSTALL_TYPES
																				  Developer)

#

# Sphinx

find_program (SPHINX_EXECUTABLE NAMES sphinx-build
			  DOC "Sphinx Documentation Builder (sphinx-doc.org)")

if (SPHINX_EXECUTABLE AND OFF)

	set (conf_version "${Limes_VERSION_MAJOR}.${Limes_VERSION_MINOR}.${Limes_VERSION_PATCH}")

	configure_file (conf.py conf.py @ONLY NEWLINE_STYLE UNIX)

	unset (conf_version)

	find_program (MAKEINFO_EXECUTABLE NAMES makeinfo DOC "makeinfo tool")
	find_program (LATEXMK_EXECUTABLE NAMES latexmk DOC "latexmk tool")

	if (MAKEINFO_EXECUTABLE)
		set (info_flag info)
	endif ()

	if (LATEX_EXECUTABLE AND LATEXMK_EXECUTABLE)
		set (latex_flag latexpdf)
	endif ()

	set (docs_output "${Limes_SOURCE_DIR}/doc/sphinx")
	set (logfiles_dir "${CMAKE_CURRENT_BINARY_DIR}/logs")

	# cmake-format: off
	foreach (format IN ITEMS html singlehtml man text linkcheck ${info_flag} ${latex_flag})
	# cmake-format: on
		set (doc_format_output "limes_doc_format_${format}")
		set (format_warnings_file "${logfiles_dir}/${format}")

		if ("${format}" STREQUAL info OR "${format}" STREQUAL latexpdf)
			# cmake-format: off
	        add_custom_command (
	            OUTPUT "${doc_format_output}"
	            COMMAND
	                "${SPHINX_EXECUTABLE}"
	                    -M "${format}"
	                    "${CMAKE_CURRENT_LIST_DIR}"
	                    "${docs_output}/${format}"
	                    -c "${CMAKE_CURRENT_BINARY_DIR}"
	                    -d "${CMAKE_CURRENT_BINARY_DIR}/doctrees"
	                    -w "${format_warnings_file}"
	            DEPENDS ${doc_format_last}
	            COMMENT "Limes Sphinx docs - building format ${format}..."
	            VERBATIM USES_TERMINAL)
	        # cmake-format: on
		else ()
			# cmake-format: off
	        add_custom_command (
	            OUTPUT "${doc_format_output}"
	            COMMAND
	                "${SPHINX_EXECUTABLE}"
	                    -c "${CMAKE_CURRENT_BINARY_DIR}"
	                    -d "${CMAKE_CURRENT_BINARY_DIR}/doctrees"
	                    -b "${format}"
	                    -w "${format_warnings_file}"
	                    "${CMAKE_CURRENT_LIST_DIR}"
	                    "${docs_output}/${format}"
	            DEPENDS ${doc_format_last}
	            COMMENT "Limes Sphinx docs - building format ${format}..."
	            VERBATIM USES_TERMINAL)
	        # cmake-format: on
		endif ()

		set_property (SOURCE "${doc_format_output}" PROPERTY SYMBOLIC 1)
		list (APPEND doc_format_outputs "${doc_format_output}")
		set (doc_format_last "${doc_format_output}")
	endforeach ()

	unset (doc_format_last)
	unset (info_flag)
	unset (latex_flag)

	if (Limes_IS_TOP_LEVEL)
		set (all_flag ALL)
	endif ()

	add_custom_target (LimesSphinx ${all_flag} DEPENDS "${doc_format_outputs}"
					   COMMENT "Building Limes Sphinx documentation...")

	unset (all_flag)

	set_property (TARGET LimesSphinx APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${logfiles_dir}"
													 "${docs_output}")

	set_target_properties (
		LimesSphinx PROPERTIES FOLDER Utility LABELS "Limes;Utility;Documentation"
							   XCODE_GENERATE_SCHEME OFF)

	add_dependencies (LimesSphinx LimesDoxygen)

	install (DIRECTORY "${docs_output}/html" COMPONENT limes_doc_sphinx_html
			 DESTINATION "${CMAKE_INSTALL_DOCDIR}" PATTERN .buildinfo EXCLUDE)

	cpack_add_component (
		limes_doc_sphinx_html DISPLAY_NAME "Limes Sphinx HTML docs"
		DESCRIPTION "Install the Limes Sphinx HTML documentation" GROUP limes_sphinx_docs)

	install (DIRECTORY "${docs_output}/singlehtml" COMPONENT limes_doc_sphinx_singlehtml
			 DESTINATION "${CMAKE_INSTALL_DOCDIR}" PATTERN .buildinfo EXCLUDE)

	cpack_add_component (
		limes_doc_sphinx_singlehtml DISPLAY_NAME "Limes single-page Sphinx HTML docs"
		DESCRIPTION "Install the Limes Sphinx single-page HTML documentation"
		GROUP limes_sphinx_docs)

	install (FILES "${docs_output}/man/limes.1" COMPONENT limes_doc_sphinx_man
			 DESTINATION "${CMAKE_INSTALL_MANDIR}")

	cpack_add_component (limes_doc_sphinx_man DISPLAY_NAME "Limes man pages"
						 DESCRIPTION "Install the Limes man pages" GROUP limes_sphinx_docs)

	if (MAKEINFO_EXECUTABLE)
		install (FILES "${docs_output}/info/texinfo/limes.info" COMPONENT limes_doc_sphinx_info
				 DESTINATION "${CMAKE_INSTALL_INFODIR}")

		cpack_add_component (limes_doc_sphinx_info DISPLAY_NAME "Limes info pages"
							 DESCRIPTION "Install the Limes info pages" GROUP limes_sphinx_docs)
	endif ()

	if (LATEX_EXECUTABLE AND LATEXMK_EXECUTABLE)
		install (FILES "${docs_output}/latexpdf/latex/limes.pdf" COMPONENT limes_doc_sphinx_pdf
				 DESTINATION "${CMAKE_INSTALL_DOCDIR}")

		cpack_add_component (
			limes_doc_sphinx_pdf DISPLAY_NAME "Limes Sphinx PDF documentation"
			DESCRIPTION "Install the Limes Sphinx PDF documentation" GROUP limes_sphinx_docs)
	endif ()

	cpack_add_component_group (
		limes_sphinx_docs DISPLAY_NAME "Limes Sphinx documentation"
		DESCRIPTION "Install all Limes Sphinx documentation" PARENT_GROUP limes_docs INSTALL_TYPES
																					 Developer)

else ()
	message (WARNING "Sphinx not found, Limes Sphinx docs cannot be built")
endif ()

#

add_custom_target (LimesDocs COMMENT "Building Limes documentation...")

add_dependencies (LimesDocs LimesDoxygen)

if (TARGET LimesSphinx)
	add_dependencies (LimesDocs LimesSphinx)
endif ()

cpack_add_component_group (
	limes_docs DISPLAY_NAME "All Limes documentation" DESCRIPTION "Install all Limes documentation"
	PARENT_GROUP limes INSTALL_TYPES Developer)
