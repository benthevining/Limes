# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (geometry)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME math
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  Fraction.h
		  fraction_staticAsserts.cpp
		  math_staticAsserts.cpp
		  mathFunctions.h
		  mathFunctions_impl.h
		  Random.cpp
		  Random.h
		  StatsComputer.h
		  matrix.h)

set (math_files ${math_files} ${geometry_files} PARENT_SCOPE)
