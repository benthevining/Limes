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
- DataStructures
- Locale
- Music
- Vecops
- MIDI
- BinaryBuilder
- Libs
- Programs
- All

]]

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

@PACKAGE_INIT@

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#

include (CMakeFindDependencyMacro)

find_dependency (Oranges 2.23.2)

#

set (
	limes_libs
	# cmake-format: sortable
	Audio
	Core
	DataStructures
	Locale
	MIDI
	Music
	Vecops)

set (limes_programs # cmake-format: sortable
					BinaryBuilder)

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
			message (WARNING " -- Limes: unknown component ${comp_name} requested!")
			set (${CMAKE_FIND_PACKAGE_NAME}_${comp_name}_FOUND FALSE)
			list (REMOVE_ITEM finding_components "${comp_name}")
		endif ()
	endforeach ()
endif ()

#

if (Audio IN_LIST finding_components)
	list (APPEND finding_components DataStructures Vecops Core)
endif ()

if (DataStructures IN_LIST finding_components)
	list (APPEND finding_components Vecops Core)
endif ()

if (Locale IN_LIST finding_components)
	list (APPEND finding_components DataStructures Vecops Core)
endif ()

if (MIDI IN_LIST finding_components)
	list (APPEND finding_components Core)
endif ()

if (Music IN_LIST finding_components)
	list (APPEND finding_components DataStructures Vecops Core)
endif ()

if (Vecops IN_LIST finding_components)
	list (APPEND finding_components Core)
endif ()

list (REMOVE_DUPLICATES finding_components)

#

list (JOIN finding_components " " all_comps_string)

message (DEBUG "Limes - config - importing components: ${all_comps_string}")

#

if (Vecops IN_LIST finding_components)
	if ("@LIMES_VECOPS_USING_IPP@") # LIMES_VECOPS_USING_IPP
		find_dependency (IPP COMPONENTS CORE S VM)
	elseif ("@LIMES_VECOPS_USING_MIPP@") # LIMES_VECOPS_USING_MIPP
		find_dependency (MIPP)
	endif ()

	if ("@LIMES_VECOPS_USING_FFTW@") # LIMES_VECOPS_USING_FFTW
		find_dependency (FFTW)
	endif ()
endif ()

# if(MIDI IN_LIST finding_components) find_dependency (MTS-ESP) endif()

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
