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
		# cmake-format: sortable
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/AtomicObjectHolder.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/NonrealtimeMutatableObject.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ObjectBase.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/RealtimeMutatableObject.h>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/RealtimeSafeObject_impl.h>
		$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeSafeObject/AtomicObjectHolder.h>
		$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeSafeObject/NonrealtimeMutatableObject.h>
		$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeSafeObject/ObjectBase.h>
		$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeSafeObject/RealtimeMutatableObject.h>
		$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeSafeObject/RealtimeSafeObject_impl.h>
	)

install (
	FILES # cmake-format: sortable
		  AtomicObjectHolder.h NonrealtimeMutatableObject.h ObjectBase.h RealtimeMutatableObject.h
		  RealtimeSafeObject_impl.h
	DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/threading/RealtimeSafeObject"
	COMPONENT limes_core_dev)

set (
	rtso_files
	# cmake-format: sortable
	threading/RealtimeSafeObject/AtomicObjectHolder.h
	threading/RealtimeSafeObject/NonrealtimeMutatableObject.h
	threading/RealtimeSafeObject/ObjectBase.h
	threading/RealtimeSafeObject/RealtimeMutatableObject.h
	threading/RealtimeSafeObject/RealtimeSafeObject_impl.h
	PARENT_SCOPE)
