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
	DIRECTORY_NAME memory
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  AlignedAllocator.h
		  alignment.h
		  array_pointer.h
		  MemoryPool.cpp
		  MemoryPool.h
		  MemoryPoolAllocator.h
		  OptionalObject.h
		  ReferenceCountedObject.cpp
		  ReferenceCountedObject.h
		  stack_allocator.h
		  StaticObjects.h
		  RawData.h
		  RawData.cpp)

set (memory_files ${memory_files} PARENT_SCOPE)
