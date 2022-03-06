# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

target_sources (
	limes_audio
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PeakFinder.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/PeakFinder.cpp>
			$<INSTALL_INTERFACE:include/limes_audio/PeakFinder.h>)

target_include_directories (limes_audio PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
