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
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/AlignedAllocator.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/alignment.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MemoryPool.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MemoryPool.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/MemoryPoolAllocator.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/OptionalObject.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ReferenceCountedObject.cpp>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ReferenceCountedObject.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/SharedObject.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/stack_allocator.h>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/StaticObjects.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/AlignedAllocator.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/alignment.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/MemoryPool.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/MemoryPoolAllocator.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/OptionalObject.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/ReferenceCountedObject.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/SharedObject.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/stack_allocator.h>
			$<INSTALL_INTERFACE:${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory/StaticObjects.h>)

install (
	FILES # cmake-format: sortable
		  AlignedAllocator.h
		  alignment.h
		  MemoryPool.h
		  MemoryPoolAllocator.h
		  OptionalObject.h
		  ReferenceCountedObject.h
		  SharedObject.h
		  stack_allocator.h
		  StaticObjects.h
	DESTINATION "${LIMES_CORE_INSTALL_INCLUDE_DIR}/memory"
	COMPONENT limes_core_dev)

set (
	memory_files
	# cmake-format: sortable
	memory/AlignedAllocator.h
	memory/alignment.h
	memory/MemoryPool.cpp
	memory/MemoryPool.h
	memory/MemoryPoolAllocator.h
	memory/OptionalObject.h
	memory/ReferenceCountedObject.cpp
	memory/ReferenceCountedObject.h
	memory/SharedObject.h
	memory/stack_allocator.h
	memory/StaticObjects.h
	PARENT_SCOPE)
