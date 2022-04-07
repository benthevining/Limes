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
	limes_vecops PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Constants.h>
						 $<INSTALL_INTERFACE:${LIMES_VECOPS_INSTALL_INCLUDE_DIR}/Constants.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/Constants.h"
		 DESTINATION "${LIMES_VECOPS_INSTALL_INCLUDE_DIR}" COMPONENT limes_vecops_dev)
