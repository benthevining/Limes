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
	pommier_files
	# cmake-format: sortable
	neon_mathfun.cpp neon_mathfun.h pommier_wrapper.cpp pommier_wrapper.h
	sse_mathfun.cpp sse_mathfun.h)

list (TRANSFORM pommier_files PREPEND pommier/)

set (pommier_files ${pommier_files} PARENT_SCOPE)

include (OrangesGeneratePlatformHeader)

if (LIMES_IGNORE_POMMIER OR (NOT (ORANGES_ARM_NEON OR ORANGES_SSE))
	OR (MSVC AND NOT ORANGES_ARM_NEON)) # MSVC doesn't compile MMX
	# intrinsics used in sse_mathfun
	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_POMMIER=0)
	return ()
endif ()

target_sources (
	limes_vecops
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/pommier_wrapper.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/pommier_wrapper.h>
		$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/pommier/pommier_wrapper.h>
	)

install (FILES pommier_wrapper.h
		 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/pommier"
		 COMPONENT limes_vecops_dev)

target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_POMMIER=1)

if (ORANGES_ARM_NEON)

	target_sources (
		limes_vecops
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/neon_mathfun.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/neon_mathfun.h>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/pommier/neon_mathfun.h>
		)

	install (FILES neon_mathfun.h
			 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/pommier"
			 COMPONENT limes_vecops_dev)

elseif (ORANGES_SSE)

	target_sources (
		limes_vecops
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/sse_mathfun.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/sse_mathfun.h>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/pommier/sse_mathfun.h>
		)

	install (FILES sse_mathfun.h
			 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/impl/pommier"
			 COMPONENT limes_vecops_dev)

endif ()
