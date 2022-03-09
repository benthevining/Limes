# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

add_subdirectory ("${CMAKE_CURRENT_LIST_DIR}/RealtimeSafeObject")

oranges_add_target_headers (
	TARGET
	limes_core
	SCOPE
	PRIVATE
	REL_PATH
	Limes/limes_core
	FILES
	RealtimeTypeTraits.h
	RealtimeSafeObject.h
	ThreadSafeObject.h)
