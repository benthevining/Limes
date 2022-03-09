# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

target_sources (limes_core PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/languages.cpp>)

oranges_add_target_headers (
	TARGET
	limes_core
	SCOPE
	PRIVATE
	REL_PATH
	Limes/limes_core
	FILES
	Algorithm.h
	TypeTraits.h
	languages.h
	StaticObjects.h
	Functions.h)
