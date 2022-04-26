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
	DIRECTORY_NAME countries
	TARGET limes_locale
	INSTALL_COMPONENT limes_locale_dev
	INSTALL_DIR "${LIMES_LOCALE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  countries.h countries.cpp)

set (countries_files ${countries_files} PARENT_SCOPE)
