# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME "harmony/intervals"
	TARGET limes_music
	INSTALL_COMPONENT limes_music_dev
	INSTALL_DIR "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  CompoundInterval.cpp
		  CompoundInterval.h
		  CompoundInterval_impl.h
		  Interval.cpp
		  Interval.h
		  Interval_impl.h
		  MicrotonalInterval.h)

set (intervals_files ${harmony/intervals_files} PARENT_SCOPE)
