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

if(NOT Oranges_FIND_COMPONENTS)
	set (Oranges_FIND_COMPONENTS Audio Core Music Vecops)
elseif(All IN_LIST Oranges_FIND_COMPONENTS)
	set (Oranges_FIND_COMPONENTS Audio Core Music Vecops)
endif()

if(Audio IN_LIST Oranges_FIND_COMPONENTS)
	include ("${CMAKE_CURRENT_LIST_DIR}/LimesAudioTargets.cmake")
endif()

if(Core IN_LIST Oranges_FIND_COMPONENTS)
	include ("${CMAKE_CURRENT_LIST_DIR}/LimesCoreTargets.cmake")
endif()

if(Music IN_LIST Oranges_FIND_COMPONENTS)
	include ("${CMAKE_CURRENT_LIST_DIR}/LimesMusicTargets.cmake")
endif()

if(Vecops IN_LIST Oranges_FIND_COMPONENTS)
	include ("${CMAKE_CURRENT_LIST_DIR}/LimesVecopsTargets.cmake")
endif()

#

set (Limes_INCLUDED TRUE)

#

check_required_components ("@PROJECT_NAME@")
