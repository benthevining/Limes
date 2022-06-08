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
	DIRECTORY_NAME time/impl
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  date_impl.h
		  month_impl.h
		  weekday_impl.h
		  year_impl.h
		  hour_impl.h
		  time_staticAsserts.cpp
		  time.cpp
		  time_impl.h)

set (impl_files ${time/impl_files} PARENT_SCOPE)

list (
	APPEND
	limes_core_proto_files
	"${CMAKE_CURRENT_LIST_DIR}/date.proto"
	"${CMAKE_CURRENT_LIST_DIR}/datetime.proto"
	"${CMAKE_CURRENT_LIST_DIR}/hour.proto"
	"${CMAKE_CURRENT_LIST_DIR}/time.proto"
	"${CMAKE_CURRENT_LIST_DIR}/month.proto"
	"${CMAKE_CURRENT_LIST_DIR}/weekday.proto"
	"${CMAKE_CURRENT_LIST_DIR}/year.proto")

set (limes_core_proto_files ${limes_core_proto_files} PARENT_SCOPE)
