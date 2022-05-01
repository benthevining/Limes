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
- BinaryBuilder
- All
- Libs
- Programs

]]

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

@PACKAGE_INIT@

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#

include (CMakeFindDependencyMacro)

find_dependency (Oranges 2.22)

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

if (NOT Limes_FIND_COMPONENTS)
	set (Limes_FIND_COMPONENTS ${limes_libs} ${limes_programs})
elseif (All IN_LIST Limes_FIND_COMPONENTS)
	set (Limes_FIND_COMPONENTS ${limes_libs} ${limes_programs})
else ()
	if (Libs IN_LIST Limes_FIND_COMPONENTS)
		list (APPEND Limes_FIND_COMPONENTS ${limes_libs})
		list (REMOVE_DUPLICATES Limes_FIND_COMPONENTS)
		list (REMOVE_ITEM Limes_FIND_COMPONENTS Libs)
	elseif (Programs IN_LIST Limes_FIND_COMPONENTS)
		list (APPEND Limes_FIND_COMPONENTS ${limes_programs})
		list (REMOVE_DUPLICATES Limes_FIND_COMPONENTS)
		list (REMOVE_ITEM Limes_FIND_COMPONENTS Programs)
	endif ()
endif ()

if (Vecops IN_LIST Limes_FIND_COMPONENTS)
	if ("@LIMES_VECOPS_USING_IPP@") # LIMES_VECOPS_USING_IPP
		find_dependency (IPP)
	elseif ("@LIMES_VECOPS_USING_MIPP@") # LIMES_VECOPS_USING_MIPP
		find_dependency (MIPP)
	endif ()

	if ("@LIMES_VECOPS_USING_FFTW@") # LIMES_VECOPS_USING_FFTW
		find_dependency (FFTW)
	endif ()
endif ()

# if(MIDI IN_LIST Limes_FIND_COMPONENTS) find_dependency (MTS-ESP) endif()

include ("${CMAKE_CURRENT_LIST_DIR}/LimesTargets.cmake")

foreach (comp_name IN LISTS Limes_FIND_COMPONENTS)
	if ("${comp_name}" IN_LIST limes_components)
		include ("${CMAKE_CURRENT_LIST_DIR}/Limes${comp_name}Targets.cmake")
		set (Limes_${comp_name}_FOUND TRUE)
	else ()
		message (WARNING " -- Limes: unknown component ${comp_name} requested!")
	endif ()
endforeach ()

#

set (Limes_Libs_FOUND TRUE)

foreach (lib_name IN LISTS limes_libs)
	if (NOT Limes_${lib_name}_FOUND)
		set (Limes_Libs_FOUND FALSE)
		break ()
	endif ()
endforeach ()

set (Limes_Programs_FOUND TRUE)

foreach (program_name IN LISTS limes_programs)
	if (NOT Limes_${program_name}_FOUND)
		set (Limes_Programs_FOUND FALSE)
		break ()
	endif ()
endforeach ()

if (Limes_Libs_FOUND AND Limes_Programs_FOUND)
	set (Limes_All_FOUND TRUE)
else ()
	set (Limes_All_FOUND FALSE)
endif ()

unset (limes_libs)
unset (limes_programs)

#

set (Limes_FOUND TRUE)

include (FeatureSummary)
include (FindPackageMessage)

set_package_properties (
	Limes PROPERTIES URL "https://github.com/benthevining/Limes"
	DESCRIPTION "C++ utilities")

find_package_message (
	Limes
	"Limes package found -- installed on system"
	"Limes (system install) [${Limes_FIND_COMPONENTS}] [${CMAKE_CURRENT_LIST_DIR}]"
	)

#

check_required_components (Limes)
