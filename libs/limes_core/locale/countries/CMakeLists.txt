# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME locale/countries
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  countries.h countries.cpp)

set (countries_files ${locale/countries_files} PARENT_SCOPE)
