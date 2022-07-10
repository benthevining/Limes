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

if (PLAT_SSE OR PLAT_ARM_NEON)
	set (pom_on ON)
else ()
	set (pom_on OFF)
endif ()

add_feature_info ("Pommier SIMD functions" "${pom_on}"
				  "Using Julien Pommier's SIMD functions for NEON/SSE")

if (NOT pom_on)
	return ()
endif ()

unset (pom_on)

set (pom_files pommier_wrapper.cpp pommier_wrapper.h)

if (PLAT_ARM_NEON)
	list (APPEND pom_files neon_mathfun.cpp neon_mathfun.h)
else ()
	list (APPEND pom_files sse_mathfun.cpp sse_mathfun.h)
endif ()

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME vecops/impl/pommier
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${pom_files})

set (pommier_files ${vecops/impl/pommier_files} PARENT_SCOPE)
