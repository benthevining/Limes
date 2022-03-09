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
	limes_music PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Interval.cpp>
						$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CompoundInterval.cpp>)

oranges_add_target_headers (
	TARGET
	limes_music
	SCOPE
	PRIVATE
	REL_PATH
	Limes/limes_music
	FILES
	Interval.h
	Interval_impl.h
	CompoundInterval.h
	CompoundInterval_impl.h
	MicrotonalInterval.h)
