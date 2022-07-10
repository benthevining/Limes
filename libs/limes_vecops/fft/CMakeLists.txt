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

list (APPEND CMAKE_MESSAGE_INDENT "FFT -- ")

oranges_add_source_files (
	DIRECTORY_NAME fft
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES fft_common.h fftw_wisdom.cpp limes_fft.cpp limes_fft.h)

set_source_files_properties (fft_common.h TARGET_DIRECTORY limes_vecops PROPERTIES ABSTRACT ON)

add_subdirectory (impl)

set (fft_files ${fft_files} ${impl_files} PARENT_SCOPE)

list (POP_BACK CMAKE_MESSAGE_INDENT)
