# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME serializing
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  serializing.h serializing.cpp json_parser.cpp encoding.cpp xml_parser.cpp)

set (serializing_files ${serializing_files} PARENT_SCOPE)

list (APPEND limes_core_proto_files "${CMAKE_CURRENT_LIST_DIR}/node.proto")

set (limes_core_proto_files ${limes_core_proto_files} PARENT_SCOPE)
