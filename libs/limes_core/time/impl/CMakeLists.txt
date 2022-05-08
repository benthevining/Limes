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
