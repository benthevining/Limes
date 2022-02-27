# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (LemonsStaticLibraryHelpers)

add_library (limes_audio STATIC)

lemons_configure_static_library (limes_audio)

target_link_libraries (limes_audio PUBLIC limes_core)

target_sources (limes_audio PUBLIC "${CMAKE_CURRENT_LIST_DIR}/limes_audio.h")

target_include_directories (limes_audio INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/streams"
				  "${CMAKE_CURRENT_BINARY_DIR}/limes_audio/streams")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/util" "${CMAKE_CURRENT_BINARY_DIR}/limes_audio/util")
add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/psola" "${CMAKE_CURRENT_BINARY_DIR}/limes_audio/psola")

lemons_sort_target_sources (limes_audio ${CMAKE_CURRENT_LIST_DIR})

add_library (Lemons::limes_audio ALIAS limes_audio)
