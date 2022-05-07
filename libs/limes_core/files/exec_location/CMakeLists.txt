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

if (APPLE)
	set (cpp_file execLoc_mac.cpp)
elseif (WIN32)
	set (cpp_file execLoc_win.cpp)
else ()
	set (cpp_file execLoc_linux.cpp)
endif ()

oranges_add_source_files (
	DIRECTORY_NAME files/exec_location
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  exec_location.h "${cpp_file}")

set (exec_files ${files/exec_location_files} PARENT_SCOPE)
