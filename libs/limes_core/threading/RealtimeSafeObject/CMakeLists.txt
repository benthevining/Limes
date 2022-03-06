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
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ObjectBase.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/AtomicObjectHolder.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NonrealtimeMutatableObject.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/RealtimeMutatableObject.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/RealtimeSafeObject_impl.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/ObjectBase.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/AtomicObjectHolder.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/NonrealtimeMutatableObject.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/RealtimeMutatableObject.h>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Limes/limes_core/RealtimeSafeObject_impl.h>)

target_include_directories (limes_core PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
