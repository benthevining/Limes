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
		# cmake-format: sortable
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CompoundInterval.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CompoundInterval.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CompoundInterval_impl.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Interval.cpp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Interval.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Interval_impl.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MicrotonalInterval.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/intervals/CompoundInterval.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/intervals/CompoundInterval_impl.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/intervals/Interval.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/intervals/Interval_impl.h>
		$<INSTALL_INTERFACE:${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/intervals/MicrotonalInterval.h>
	)

install (
	FILES # cmake-format: sortable
		  CompoundInterval.h CompoundInterval_impl.h Interval.h Interval_impl.h MicrotonalInterval.h
	DESTINATION "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}/harmony/intervals" COMPONENT limes_music_dev)

set (
	intervals_files
	# cmake-format: sortable
	harmony/intervals/CompoundInterval.cpp
	harmony/intervals/CompoundInterval.h
	harmony/intervals/CompoundInterval_impl.h
	harmony/intervals/Interval.cpp
	harmony/intervals/Interval.h
	harmony/intervals/Interval_impl.h
	harmony/intervals/MicrotonalInterval.h
	PARENT_SCOPE)
