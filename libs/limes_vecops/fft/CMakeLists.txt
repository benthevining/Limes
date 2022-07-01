# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (FeatureSummary)

set_package_properties (FFTW PROPERTIES TYPE RECOMMENDED PURPOSE "FFT acceleration with FFTW")

#

set (files fft_common.h fftw_wisdom.cpp limes_fft.cpp limes_fft.h)

if (LIMES_USE_FFTW)
	find_package (FFTW)

	if (TARGET FFTW::FFTW)
		target_link_libraries (limes_vecops PRIVATE FFTW::FFTW)

		target_compile_definitions (limes_vecops PRIVATE LIMES_VECOPS_USE_FFTW=1)

		set (files ${files} fftw_fft.h fftw_fft.cpp)

		set (LIMES_VECOPS_USING_FFTW ON CACHE INTERNAL "")

		set (impl_name FFTW)
	endif ()
endif ()

if (NOT LIMES_USE_FFTW OR NOT TARGET FFTW::FFTW)
	set (LIMES_VECOPS_USING_FFTW OFF CACHE INTERNAL "")

	get_property (vecops_impl GLOBAL PROPERTY LIMES_VECOPS_IMPLEMENTATION)

	if ("${vecops_impl}" MATCHES vDSP)
		set (files ${files} vdsp_fft.h vdsp_fft.cpp)
		set (impl_name vDSP)
	elseif ("${vecops_impl}" STREQUAL IPP)
		set (files ${files} ipp_fft.h ipp_fft.cpp)
		set (impl_name IPP)
	else ()
		set (files ${files} fallback_fft.h fallback_fft.cpp)
		set (impl_name Fallback)
	endif ()
endif ()

message (DEBUG " -- limes_vecops - using ${impl_name} for FFT")

add_feature_info (FFTW "${impl_name} STREQUAL FFTW"
				  "Using the ${impl_name} backend for the limes_vecops FFT")

set_property (GLOBAL PROPERTY LIMES_FFT_IMPLEMENTATION "${impl_name}")

unset (impl_name)

#

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME fft
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${files})

set_source_files_properties (fft_common.h TARGET_DIRECTORY limes_vecops PROPERTIES ABSTRACT ON)

set (fft_files ${fft_files} PARENT_SCOPE)
