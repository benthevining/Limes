# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

set_source_files_properties (ObjectBase.h TARGET_DIRECTORY limes_core
							 PROPERTIES ABSTRACT ON)

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME "threading/RealtimeSafeObject"
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  AtomicObjectHolder.h NonrealtimeMutatableObject.h ObjectBase.h
		  RealtimeMutatableObject.h RealtimeSafeObject_impl.h)

set (rtso_files ${threading/RealtimeSafeObject_files} PARENT_SCOPE)
