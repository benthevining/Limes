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
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/mathHelpers.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Fraction.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Fraction_impl.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/mathHelpers.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/Fraction.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/Fraction_impl.h>)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/mathHelpers.h" "${CMAKE_CURRENT_LIST_DIR}/Fraction.h"
			   "${CMAKE_CURRENT_LIST_DIR}/Fraction_impl.h"
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}" COMPONENT limes_core_dev)
