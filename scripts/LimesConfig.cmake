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

@PACKAGE_INIT@

set_and_check (LIMES_ROOT_DIR "@PACKAGE_LIMES_ROOT_DIR@")

include ("${CMAKE_CURRENT_LIST_DIR}/LimesMacros.cmake")

#

include (CMakeFindDependencyMacro)

if(NOT Oranges_FOUND)
	find_dependency (Oranges)
endif()

if("@LIMES_VECOPS_USING_IPP@") # LIMES_VECOPS_USING_IPP
	if(NOT IPP_FOUND)
		find_dependency (IPP)
	endif()
endif()

if("@LIMES_VECOPS_USING_MIPP@") # LIMES_VECOPS_USING_MIPP
	if(NOT MIPP_FOUND)
		find_dependency (MIPP)
	endif()
endif()

#

include ("${CMAKE_CURRENT_LIST_DIR}/LimesTargets.cmake")

set (limes_components Audio Core Music Vecops DataStructures Locale)

if(NOT Limes_FIND_COMPONENTS)
	set (Limes_FIND_COMPONENTS ${limes_components})
elseif(All IN_LIST Limes_FIND_COMPONENTS)
	set (Limes_FIND_COMPONENTS ${limes_components})
endif()

foreach(comp_name IN LISTS Limes_FIND_COMPONENTS)
	if("${comp_name}" IN_LIST limes_components)
		include ("${CMAKE_CURRENT_LIST_DIR}/Limes${comp_name}Targets.cmake")
	else()
		message (WARNING " -- Limes: unknown component ${comp_name} requested!")
	endif()
endforeach()

unset (limes_components)

#

set (Limes_FOUND TRUE)

include (FeatureSummary)
include (FindPackageMessage)

set_package_properties (Limes PROPERTIES URL "https://github.com/benthevining/Limes"
						DESCRIPTION "C++ utilities")

find_package_message (Limes "Limes package found -- installed on system" "Limes (system install)")

#

check_required_components (Limes)
