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
- All
- Audio
- Core
- DataStructures
- Locale
- Music
- Vecops
- BinaryBuilder

]]

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

@PACKAGE_INIT@

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#

include (CMakeFindDependencyMacro)

find_dependency (Oranges)

if("@LIMES_VECOPS_USING_IPP@") # LIMES_VECOPS_USING_IPP
	find_dependency (IPP)
endif()

if("@LIMES_VECOPS_USING_MIPP@") # LIMES_VECOPS_USING_MIPP
	find_dependency (MIPP)
endif()

if("@LIMES_VECOPS_USING_FFTW@") # LIMES_VECOPS_USING_FFTW
	find_dependency (FFTW)
endif()

#

set (
	limes_components
	Audio
	MIDI
	Core
	Music
	Vecops
	DataStructures
	Locale
	BinaryBuilder)

if(NOT Limes_FIND_COMPONENTS)
	set (Limes_FIND_COMPONENTS ${limes_components})
elseif(All IN_LIST Limes_FIND_COMPONENTS)
	set (Limes_FIND_COMPONENTS ${limes_components})
endif()

# if(MIDI IN_LIST Limes_FIND_COMPONENTS) find_dependency (MTS-ESP) endif()

foreach(comp_name IN LISTS Limes_FIND_COMPONENTS)
	if("${comp_name}" IN_LIST limes_components)
		include ("${CMAKE_CURRENT_LIST_DIR}/Limes${comp_name}Targets.cmake")
		set (Limes_${comp_name}_FOUND TRUE)
	else()
		message (WARNING " -- Limes: unknown component ${comp_name} requested!")
	endif()
endforeach()

unset (limes_components)

include ("${CMAKE_CURRENT_LIST_DIR}/LimesTargets.cmake")

#

set (Limes_FOUND TRUE)

include (FeatureSummary)
include (FindPackageMessage)

set_package_properties (Limes PROPERTIES URL "https://github.com/benthevining/Limes"
						DESCRIPTION "C++ utilities")

find_package_message (Limes "Limes package found -- installed on system"
					  "Limes (system install) [${Limes_FIND_COMPONENTS}]")

#

check_required_components (Limes)
