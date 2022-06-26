# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

find_package (JUCE 7.0.0 REQUIRED)

set_package_properties (JUCE PROPERTIES TYPE REQUIRED
						PURPOSE "C++ framework for cross platform GUI, audio & plugin development")

add_subdirectory (modules)

cmake_dependent_option (LIMES_BUILD_EXAMPLE_PLUGINS "Build the Limes example plugins"
						"${PROJECT_IS_TOP_LEVEL}" "NOT LIMES_MINIMAL" OFF)

if (LIMES_BUILD_EXAMPLE_PLUGINS)
	add_subdirectory (plugins)
endif ()

add_feature_info ("Example JUCE plugins" "${LIMES_BUILD_EXAMPLE_PLUGINS}"
				  "Build the example JUCE plugins using the Limes JUCE modules")

# todo -- JUCE tests...?
