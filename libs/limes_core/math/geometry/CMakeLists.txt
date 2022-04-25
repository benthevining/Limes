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
	PRIVATE # cmake-format: sortable
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/area.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/perimeter.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/volume.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/geometry/area.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/geometry/perimeter.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/geometry/volume.h>)

install (FILES # cmake-format: sortable
			   area.h perimeter.h volume.h
		 DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/math/geometry" COMPONENT limes_core_dev)

set (geometry_files # cmake-format: sortable
					math/geometry/area.h math/geometry/perimeter.h math/geometry/volume.h
	 PARENT_SCOPE)
