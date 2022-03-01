# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (LemonsDefaultProjectSettings)
include (LemonsDefaultTarget)

add_library (limes_vecops INTERFACE)

target_link_libraries (limes_vecops INTERFACE LemonsDefaultTarget)

target_sources (
	limes_vecops
	INTERFACE "${CMAKE_CURRENT_LIST_DIR}/limes_vecops.h"
			  "${CMAKE_CURRENT_LIST_DIR}/util/Platform.h"
			  "${CMAKE_CURRENT_LIST_DIR}/util/Constants.h")

target_include_directories (limes_vecops INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

if(APPLE)
	target_compile_definitions (limes_vecops INTERFACE LIMES_VECOPS_USE_VDSP=1)
	target_sources (limes_vecops INTERFACE "${CMAKE_CURRENT_LIST_DIR}/impl/vdsp.h")
else()
	# if on an Intel processor, check for IPP...

	target_sources (limes_vecops INTERFACE "${CMAKE_CURRENT_LIST_DIR}/impl/fallback.h")
endif()

lemons_sort_target_sources (limes_vecops ${CMAKE_CURRENT_LIST_DIR})

set_target_properties (limes_vecops PROPERTIES LINKER_LANGUAGE CXX)

add_library (Lemons::limes_vecops ALIAS limes_vecops)
