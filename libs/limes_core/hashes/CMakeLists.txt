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
	DIRECTORY_NAME hashes
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  hash.h
		  md5.cpp
		  md5.h
		  sha1.cpp
		  sha1.h
		  hash.cpp
		  sha224.cpp
		  sha224.h
		  sha256.cpp
		  sha256.h
		  sha384.cpp
		  sha384.h
		  sha512.cpp
		  sha512.h
		  common.h
		  common.cpp)

set (hashes_files ${hashes_files} PARENT_SCOPE)
