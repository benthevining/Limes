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

set_source_files_properties (fft_common.h TARGET_DIRECTORY limes_vecops PROPERTIES ABSTRACT ON)

#

target_sources (
	limes_vecops
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fft_common.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fftw_wisdom.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_fft.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/limes_fft.h>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/fft/fft_common.h>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/fft/limes_fft.h>)

install (FILES # cmake-format: sortable
			   fft_common.h limes_fft.h DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/fft"
		 COMPONENT limes_vecops_dev)

set (
	fft_files
	# cmake-format: sortable
	fallback_fft.cpp
	fallback_fft.h
	fft_common.h
	fftw_fft.cpp
	fftw_fft.h
	fftw_wisdom.cpp
	ipp_fft.cpp
	ipp_fft.h
	limes_fft.cpp
	limes_fft.h
	vdsp_fft.cpp
	vdsp_fft.h)

list (TRANSFORM fft_files PREPEND fft/)

set (fft_files ${fft_files} PARENT_SCOPE)

#

function(_limes_fft_end_callback)

	add_feature_info (FFTW "TARGET FFTW::FFTW AND NOT LIMES_IGNORE_FFTW"
					  "Accelerated FFT using FFTW")

	target_sources (
		limes_vecops
		PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${LIMES_FFT_SOURCE_NAME}.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${LIMES_FFT_SOURCE_NAME}.h>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/fft/${LIMES_FFT_SOURCE_NAME}.h>)

	install (FILES "${LIMES_FFT_SOURCE_NAME}.h"
			 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/fft" COMPONENT limes_vecops_dev)

endfunction()

cmake_language (DEFER CALL _limes_fft_end_callback)

#

if(NOT LIMES_IGNORE_FFTW)
	find_package (FFTW MODULE QUIET)

	if(TARGET FFTW::FFTW)
		message (DEBUG " -- limes_vecops - using FFTW for FFT")
		target_link_libraries (limes_vecops PRIVATE FFTW::FFTW)
		target_compile_definitions (limes_vecops PRIVATE LIMES_VECOPS_USE_FFTW=1)

		set (LIMES_FFT_SOURCE_NAME fftw_fft)

		set (LIMES_VECOPS_USING_FFTW ON CACHE INTERNAL "")

		return ()
	endif()
endif()

get_target_property (vecops_impl limes_vecops VECOPS_IMPLEMENTATION)

if("${vecops_impl}" MATCHES vDSP)
	set (LIMES_FFT_SOURCE_NAME vdsp_fft)
elseif("${vecops_impl}" STREQUAL IPP)
	set (LIMES_FFT_SOURCE_NAME ipp_fft)
else()
	set (LIMES_FFT_SOURCE_NAME fallback_fft)
endif()

unset (vecops_impl)
