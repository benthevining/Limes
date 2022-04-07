# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

option (LIMES_IGNORE_FFTW "Ignore FFTW for limes_vecops" OFF)

mark_as_advanced (FORCE LIMES_IGNORE_FFTW)

include (FeatureSummary)

set_package_properties (FFTW PROPERTIES TYPE RECOMMENDED PURPOSE "FFT acceleration with FFTW")

if(NOT LIMES_IGNORE_FFTW)
	find_package (FFTW MODULE QUIET)

	if(TARGET FFTW::FFTW)
		message (DEBUG " -- limes_vecops - using FFTW for FFT")
		target_link_libraries (limes_vecops PRIVATE FFTW::FFTW)
		target_compile_definitions (limes_vecops PRIVATE LIMES_VECOPS_USE_FFTW=1)
	endif()
endif()

target_sources (
	limes_vecops
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fft.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fft.cpp>
			$<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/fft.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/fft.h" DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
		 COMPONENT limes_vecops_dev)

add_feature_info (FFTW "TARGET FFTW::FFTW" "Accelerated FFT using FFTW")
