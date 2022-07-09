# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

set (
	files
	# cmake-format: sortable
	check_impl.h
	fallback_impl.cpp
	fallback_impl.h
	fp_status.cpp
	fp_status.h
	phasor.cpp
	phasor.h
	vecops_macros.h)

if (LIMES_USE_VDSP)
	list (APPEND files vdsp.h)
elseif (LIMES_USE_IPP)
	list (APPEND files ipp.h)
elseif (LIMES_USE_MIPP)
	list (APPEND files mipp.h)
else ()
	list (APPEND files fallback.h)
endif ()

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME impl
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${files})

add_subdirectory (pommier)

set (impl_files ${impl_files} ${pommier_files} PARENT_SCOPE)
