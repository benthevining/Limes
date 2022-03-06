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
			$<INSTALL_INTERFACE:include/limes_core/mathHelpers.h>
			$<INSTALL_INTERFACE:include/limes_core/Fraction.h>
			$<INSTALL_INTERFACE:include/limes_core/Fraction_impl.h>)

target_include_directories (limes_core PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
