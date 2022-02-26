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
	PRIVATE RealtimeTypeTraits.h
			RealtimeSafeObject.h
			ThreadSafeObject.h
			RealtimeSafeObject/ObjectBase.h
			RealtimeSafeObject/AtomicObjectHolder.h
			RealtimeSafeObject/NonrealtimeMutatableObject.h
			RealtimeSafeObject/RealtimeMutatableObject.h
			RealtimeSafeObject/RealtimeSafeObject_impl.h)

target_include_directories (limes_core PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
