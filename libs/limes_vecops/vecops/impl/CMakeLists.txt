# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

set (files fallback_impl.cpp fallback_impl.h phasor.cpp phasor.h)

list (APPEND CMAKE_MESSAGE_INDENT "vecops/impl dir -- ")

#

if (LIMES_USE_VDSP)

	list (APPEND files vdsp.h)

	message (DEBUG "Using vDSP")

elseif (LIMES_USE_IPP)

	list (APPEND files ipp.h)

	message (DEBUG "Using IPP")

elseif (LIMES_USE_MIPP)

	list (APPEND files mipp.h)

elseif (LIMES_USE_NE10)

	list (APPEND files ne10.h ne10.cpp)

	message (DEBUG "Using NE10")

else ()

	list (APPEND files fallback.h)

	message (DEBUG "Using fallback")

endif ()

#

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME vecops/impl
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${files})

add_subdirectory (pommier)

set (impl_files ${vecops/impl_files} ${pommier_files} PARENT_SCOPE)

list (POP_BACK CMAKE_MESSAGE_INDENT)
