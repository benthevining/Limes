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
	limes_core
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/time_utils.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/build_date.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/time/time_utils.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/time/build_date.h>)

install (FILES time_utils.h build_date.h DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/time"
		 COMPONENT limes_core_dev)
