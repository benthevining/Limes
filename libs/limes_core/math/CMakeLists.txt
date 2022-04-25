# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory (geometry)

target_sources (
	limes_core
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Fraction.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Fraction_impl.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/fraction_staticAsserts.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/math_staticAsserts.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/mathHelpers.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Random.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Random.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/Fraction.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/Fraction_impl.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/mathHelpers.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/Random.h>)

install (FILES # cmake-format: sortable
			   Fraction.h Fraction_impl.h mathHelpers.h Random.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/math" COMPONENT limes_core_dev)

set (
	math_files
	# cmake-format: sortable
	${geometry_files}
	math/Fraction.h
	math/Fraction_impl.h
	math/fraction_staticAsserts.cpp
	math/math_staticAsserts.cpp
	math/mathHelpers.h
	math/Random.cpp
	math/Random.h
	PARENT_SCOPE)
