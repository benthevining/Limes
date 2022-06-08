# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (version)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME misc
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  Algorithm.h Functions.h IgnoreUnused.h preprocessor.h streams.h streams.cpp)

set (misc_files ${misc_files} ${version_files} PARENT_SCOPE)

set (misc_proto_files ${version_proto_files} PARENT_SCOPE)
