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
	limes_vecops
	INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Constants.h>
			  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Platform.h>
			  $<INSTALL_INTERFACE:include/limes_vecops/Constants.h>
			  $<INSTALL_INTERFACE:include/limes_vecops/Platform.h>)
