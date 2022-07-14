# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

#[[

Package configuration file for Limes, version @Limes_VERSION@.

The following components may be specified in calls to find_package:
- Audio
- Core
- Music
- Vecops
- MIDI
- BinaryBuilder
- FileUtil
- Libs
- Programs
- All

]]

cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

@PACKAGE_INIT@

#

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include (CMakeFindDependencyMacro)

find_dependency (Oranges 3.3.0)

#

set (limes_libs # cmake-format: sortable
				Audio Core MIDI Music Vecops)

set (limes_programs # cmake-format: sortable
					BinaryBuilder FileUtil)

set (limes_components ${limes_libs} ${limes_programs})

if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
	set (finding_components ${limes_components})
elseif (All IN_LIST ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
	set (finding_components ${limes_components})
else ()
	set (finding_components ${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS})

	if (Libs IN_LIST finding_components)
		list (APPEND finding_components ${limes_libs})
		list (REMOVE_DUPLICATES finding_components)
		list (REMOVE_ITEM finding_components Libs)
	endif ()

	if (Programs IN_LIST finding_components)
		list (APPEND finding_components ${limes_programs})
		list (REMOVE_DUPLICATES finding_components)
		list (REMOVE_ITEM finding_components Programs)
	endif ()

	foreach (comp_name IN LISTS finding_components)
		if (NOT "${comp_name}" IN_LIST limes_components)
			if (NOT ${CMAKE_FIND_PACKAGE_NAME}_QUIETLY)
				message (WARNING " -- Limes: unknown component ${comp_name} requested!")
			endif ()

			set (${CMAKE_FIND_PACKAGE_NAME}_${comp_name}_FOUND FALSE)
			list (REMOVE_ITEM finding_components "${comp_name}")
		endif ()
	endforeach ()
endif ()

#

set (any_limes_libs OFF)

if (Audio IN_LIST finding_components)
	list (APPEND finding_components DataStructures Vecops Core)
	set (any_limes_libs ON)
endif ()

if (MIDI IN_LIST finding_components)
	list (APPEND finding_components Core)
	set (any_limes_libs ON)
endif ()

if (Music IN_LIST finding_components)
	list (APPEND finding_components DataStructures Vecops Core)
	set (any_limes_libs ON)
endif ()

if (Vecops IN_LIST finding_components)
	list (APPEND finding_components Core)
	set (any_limes_libs ON)
endif ()

list (REMOVE_DUPLICATES finding_components)

#

list (JOIN finding_components " " all_comps_string)

if (NOT ${CMAKE_FIND_PACKAGE_NAME}_QUIETLY)
	message (DEBUG "Limes - config - importing components: ${all_comps_string}")
endif ()

#

if (Vecops IN_LIST finding_components)
	if ("@LIMES_USE_IPP@") # LIMES_USE_IPP
		find_dependency (
			IPP
			7
			MODULE
			COMPONENTS
			CORE
			S
			VM)
	elseif ("@LIMES_USE_MIPP@") # LIMES_USE_MIPP
		find_dependency (MIPP MODULE)
	elseif ("@LIMES_USE_NE10@") # LIMES_USE_NE10
		find_dependency (NE10 MODULE)
	endif ()

	if ("@LIMES_USE_FFTW@") # LIMES_USE_FFTW
		find_dependency (FFTW MODULE)
	endif ()

	if ("@LIMES_USE_LIBSAMPLERATE@") # LIMES_USE_LIBSAMPLERATE
		find_dependency (SampleRate MODULE)
	endif ()
endif ()

# if(MIDI IN_LIST finding_components) find_dependency (MTS-ESP) endif()

if (any_limes_libs)
	# this file imports the default library target that all other library targets link against
	include ("${CMAKE_CURRENT_LIST_DIR}/LimesDefaultLibraryTarget.cmake")
endif ()

foreach (comp_name IN LISTS finding_components)
	include ("${CMAKE_CURRENT_LIST_DIR}/Limes${comp_name}Targets.cmake")
	set (${CMAKE_FIND_PACKAGE_NAME}_${comp_name}_FOUND TRUE)
endforeach ()

#

set (${CMAKE_FIND_PACKAGE_NAME}_Libs_FOUND TRUE)

foreach (lib_name IN LISTS limes_libs)
	if (NOT ${CMAKE_FIND_PACKAGE_NAME}_${lib_name}_FOUND)
		set (${CMAKE_FIND_PACKAGE_NAME}_Libs_FOUND FALSE)
		break ()
	endif ()
endforeach ()

if (${CMAKE_FIND_PACKAGE_NAME}_Libs_FOUND)
	# this file imports the aggregate Limes::Limes target
	include ("${CMAKE_CURRENT_LIST_DIR}/LimesLibraryTargets.cmake")
endif ()

set (${CMAKE_FIND_PACKAGE_NAME}_Programs_FOUND TRUE)

foreach (program_name IN LISTS limes_programs)
	if (NOT ${CMAKE_FIND_PACKAGE_NAME}_${program_name}_FOUND)
		set (${CMAKE_FIND_PACKAGE_NAME}_Programs_FOUND FALSE)
		break ()
	endif ()
endforeach ()

if (${CMAKE_FIND_PACKAGE_NAME}_Libs_FOUND AND ${CMAKE_FIND_PACKAGE_NAME}_Programs_FOUND)
	set (${CMAKE_FIND_PACKAGE_NAME}_All_FOUND TRUE)
else ()
	set (${CMAKE_FIND_PACKAGE_NAME}_All_FOUND FALSE)
endif ()

#

set (${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE)

include (FeatureSummary)
include (FindPackageMessage)

set_package_properties ("${CMAKE_FIND_PACKAGE_NAME}" PROPERTIES
						URL "https://github.com/benthevining/Limes" DESCRIPTION "C++ utilities")

find_package_message ("${CMAKE_FIND_PACKAGE_NAME}" "Limes package found -- installed on system"
					  "Limes (system install) [${finding_components}] [${CMAKE_CURRENT_LIST_DIR}]")

#

check_required_components ("${CMAKE_FIND_PACKAGE_NAME}")
