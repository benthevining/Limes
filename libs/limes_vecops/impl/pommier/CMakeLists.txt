# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesGeneratePlatformHeader)

if ((NOT (ORANGES_ARM_NEON OR ORANGES_SSE)) OR (MSVC AND NOT ORANGES_ARM_NEON)) # MSVC doesn't
	# compile MMX intrinsics used in sse_mathfun
	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_POMMIER=0)

	set (LIMES_USE_POMMIER FALSE
		 CACHE BOOL "Use the Pommier SIMD extensions for limes_vecops (SSE or NEON)" FORCE)

	return ()
endif ()

target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_POMMIER=1)

set (pom_files pommier_wrapper.cpp pommier_wrapper.h)

if (ORANGES_ARM_NEON)
	set (pom_files ${pom_files} neon_mathfun.cpp neon_mathfun.h)
else ()
	set (pom_files ${pom_files} sse_mathfun.cpp sse_mathfun.h)
endif ()

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME impl/pommier
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${pom_files})

set (pommier_files ${impl/pommier_files} PARENT_SCOPE)
