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
	limes_music
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Interval.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Interval.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Interval_impl.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CompoundInterval.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CompoundInterval.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CompoundInterval_impl.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MicrotonalInterval.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_music/Interval.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_music/Interval_impl.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_music/CompoundInterval.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_music/CompoundInterval_impl.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_music/MicrotonalInterval.h>)

target_include_directories (limes_music PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
