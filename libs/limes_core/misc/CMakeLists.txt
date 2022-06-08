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
	DIRECTORY_NAME misc
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  Algorithm.h
		  Functions.h
		  IgnoreUnused.h
		  preprocessor.h
		  streams.h
		  Version.cpp
		  Version.h
		  streams.cpp)

set (misc_files ${misc_files} PARENT_SCOPE)

list (APPEND limes_core_proto_files "${CMAKE_CURRENT_LIST_DIR}/version.proto")

set (limes_core_proto_files ${limes_core_proto_files} PARENT_SCOPE)
