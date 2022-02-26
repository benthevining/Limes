# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include_guard (GLOBAL)

#

include (LemonsStaticLibraryHelpers)

add_library (limes_core STATIC)

lemons_configure_static_library (limes_core)

include (limes_vecops)

target_link_libraries (limes_core PUBLIC limes_vecops)

target_sources (limes_core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/limes_core.h")

target_include_directories (limes_core INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/data_structures"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_core/data_structures")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/math" "${CMAKE_CURRENT_BINARY_DIR}/limes_core/math")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/misc" "${CMAKE_CURRENT_BINARY_DIR}/limes_core/misc")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/threading"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_core/threading")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/text" "${CMAKE_CURRENT_BINARY_DIR}/limes_core/text")

lemons_sort_target_sources (limes_core ${CMAKE_CURRENT_LIST_DIR})

add_library (Lemons::limes_core ALIAS limes_core)
