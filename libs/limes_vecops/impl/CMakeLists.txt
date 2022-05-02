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

target_sources (
	limes_vecops
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fallback_impl.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fallback_impl.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fp_status.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/phasor.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/phasor.h>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/fallback_impl.h>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/phasor.h>)

install (FILES fallback_impl.h phasor.h DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl"
		 COMPONENT limes_vecops_dev)

add_subdirectory (pommier)

set (
	impl_files
	# cmake-format: sortable
	${pommier_files}
	fallback.h
	fallback_impl.cpp
	fallback_impl.h
	fp_status.cpp
	ipp.h
	mipp.h
	phasor.cpp
	phasor.h
	vdsp.h)

list (TRANSFORM impl_files PREPEND impl/)

set (impl_files ${impl_files} PARENT_SCOPE)
