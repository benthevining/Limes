# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

define_property (
	GLOBAL
	PROPERTY LIMES_RESAMPLER_IMPLEMENTATION
	BRIEF_DOCS "Name of the resampler implementation"
	FULL_DOCS
		"Name of the underlying resampler implementation used by limes_vecops. May be libsamplerate, IPP, or Fallback."
	)

option (LIMES_USE_LIBSAMPLERATE "Use libsamplerate for resampling" ON)

mark_as_advanced (FORCE LIMES_USE_LIBSAMPLERATE)

set (files # cmake-format: sortable
		   limes_resampler.cpp limes_resampler.h resampler_common.h)

include (FeatureSummary)

set_package_properties (SampleRate PROPERTIES TYPE RECOMMENDED
						PURPOSE "Resampling acceleration with libsamplerate")

if (LIMES_USE_LIBSAMPLERATE)
	find_package (SampleRate MODULE)
else ()
	set (SampleRate_FOUND OFF)
endif ()

if (SampleRate_FOUND AND TARGET SampleRate::samplerate)
	target_link_libraries (limes_vecops PRIVATE SampleRate::samplerate)

	list (APPEND files lsr_resampler.h lsr_resampler.cpp)

	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_LIBSAMPLERATE=1)

	set (LIMES_USE_LIBSAMPLERATE ON CACHE BOOL "Use libsamplerate for resampling" FORCE)

	set (impl_name libsamplerate)
else ()
	target_compile_definitions (limes_vecops PUBLIC LIMES_VECOPS_USE_LIBSAMPLERATE=0)

	set (LIMES_USE_LIBSAMPLERATE OFF CACHE BOOL "Use libsamplerate for resampling" FORCE)

	if (LIMES_USE_IPP)
		list (APPEND files ipp_resampler.h ipp_resampler.cpp)
		set (impl_name IPP)
	else ()
		list (APPEND files fallback_resampler.h fallback_resampler.cpp)
		set (impl_name Fallback)
	endif ()
endif ()

message (DEBUG " -- limes_vecops - using ${impl_name} for resampling")

add_feature_info (libsamplerate "${LIMES_USE_LIBSAMPLERATE}"
				  "Using the ${impl_name} backend for the limes_vecops resampler")

set_property (GLOBAL PROPERTY LIMES_RESAMPLER_IMPLEMENTATION "${impl_name}")

unset (impl_name)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME resampling
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES ${files})

set_source_files_properties (resampler_common.h TARGET_DIRECTORY limes_vecops PROPERTIES ABSTRACT
																						 ON)

set (resampling_files ${resampling_files} PARENT_SCOPE)
