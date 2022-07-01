# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include ("${CMAKE_CURRENT_LIST_DIR}/SelectImpl.cmake")

add_subdirectory (pommier)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME impl
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  fallback_impl.cpp fallback_impl.h fp_status.cpp phasor.h phasor.cpp vecops_macros.h)

set (impl_files ${impl_files} ${pommier_files} PARENT_SCOPE)
