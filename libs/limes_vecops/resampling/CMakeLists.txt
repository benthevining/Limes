# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (impl)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME resampling
	TARGET limes_vecops
	INSTALL_COMPONENT limes_vecops_dev
	INSTALL_DIR "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}"
	FILES limes_resampler.cpp limes_resampler.h resampler_common.h)

set_source_files_properties (resampler_common.h TARGET_DIRECTORY limes_vecops PROPERTIES ABSTRACT
																						 ON)

set (resampling_files ${resampling_files} ${impl_files} PARENT_SCOPE)
