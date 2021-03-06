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
include (CMakeDependentOption)

define_property (
	GLOBAL
	PROPERTY LIMES_FFT_IMPLEMENTATION
	BRIEF_DOCS "Name of the FFT implementation"
	FULL_DOCS
		"Name of the underlying FFT implementation library used by limes_vecops. May be vDSP, IPP, FFTW, or Fallback."
	)

set (fftw_description "Use FFTW for limes_vecops")
set (pffft_description "Use pffft for limes_vecops")

option (LIMES_USE_FFTW "${fftw_description}" ON)

cmake_dependent_option (LIMES_USE_PFFFT "${pffft_description}" OFF "NOT LIMES_USE_FFTW" OFF)

mark_as_advanced (FORCE LIMES_USE_FFTW LIMES_USE_PFFFT)

set_package_properties (FFTW PROPERTIES TYPE RECOMMENDED PURPOSE "FFT acceleration with FFTW")
set_package_properties (pffft PROPERTIES TYPE RECOMMENDED PURPOSE "Optimized FFT library")

if (LIMES_USE_FFTW)
	find_package (FFTW MODULE)
endif ()

if (LIMES_USE_FFTW AND FFTW_FOUND AND TARGET FFTW::FFTW)

	set (files fftw_fft.h fftw_fft.cpp)

	target_link_libraries (limes_vecops PUBLIC FFTW::FFTW)

	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_FFTW=1
													LIMES_VECOPS_USE_PFFFT=0)

	set (impl_name FFTW)

	set (LIMES_USE_PFFFT OFF CACHE BOOL "${pffft_description}" FORCE)

else ()

	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_FFTW=0)

	set (LIMES_USE_FFTW OFF CACHE BOOL "${fftw_description}" FORCE)

	find_package (pffft MODULE)

	if (pffft_FOUND)

		set (files pffft_fft.cpp pffft_fft.h)

		target_link_libraries (limes_vecops PUBLIC pffft::pffft)

		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_PFFFT=1)

		set (impl_name pffft)

	else ()

		target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_PFFFT=0)

		set (LIMES_USE_PFFFT OFF CACHE BOOL "${pffft_description}" FORCE)

		if (LIMES_USE_VDSP)
			set (files vdsp_fft.h vdsp_fft.cpp)
			set (impl_name vDSP)
		elseif (LIMES_USE_IPP)
			set (files ipp_fft.h ipp_fft.cpp)
			set (impl_name IPP)
		else ()
			set (files fallback_fft.h fallback_fft.cpp)
			set (impl_name Fallback)
		endif ()

	endif ()
endif ()

unset (fftw_description)
unset (pffft_description)

message (VERBOSE " -- limes_vecops - using ${impl_name} for FFT")

add_feature_info (FFTW LIMES_USE_FFTW "Using the ${impl_name} backend for the limes_vecops FFT")

set_property (GLOBAL PROPERTY LIMES_FFT_IMPLEMENTATION "${impl_name}")

unset (impl_name)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME fft/impl
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${files})

set (impl_files ${fft/impl_files} PARENT_SCOPE)
