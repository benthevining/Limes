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
	DIRECTORY_NAME time
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  build_date.h
		  year.h
		  month.h
		  weekday.h
		  date.h
		  time_utils.h
		  hour.h
		  time.h
		  DateTime.h)

set (time_files ${time_files} ${impl_files} PARENT_SCOPE)

set (time_proto_files time/time.proto PARENT_SCOPE)
