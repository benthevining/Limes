/*
 * ======================================================================================
 *  __    ____  __  __  ____  ___
 * (  )  (_  _)(  \/  )( ___)/ __)
 *  )(__  _)(_  )    (  )__) \__ \
 * (____)(____)(_/\/\_)(____)(___/
 *
 *  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

#include <limes_platform.h>

#ifdef __has_include
#	define LIMES_HAS_INCLUDE(file) __has_include(file)
#else
#	define LIMES_HAS_INCLUDE(file) 0
#endif

#ifdef __has_attribute
#	define LIMES_HAS_ATTRIBUTE(attribute) __has_attribute (attribute)
#else
#	define LIMES_HAS_ATTRIBUTE(attribute) 0
#endif


#if LIMES_MSVC
#	define LIMES_BLOCK_WITH_FORCED_SEMICOLON(statement)         \
		__pragma (warning (push))                                \
			__pragma (warning (disable : 4127)) do { statement } \
		while (false)                                            \
		__pragma (warning (pop))
#else
#	define LIMES_BLOCK_WITH_FORCED_SEMICOLON(statement) \
		do {                                             \
			statement                                    \
		} while (false)
#endif


#define LIMES_MAKE_STRING_HELPER(str) #str

#define LIMES_MAKE_STRING(str) LIMES_MAKE_STRING_HELPER (str)


#define LIMES_CONCATENATE_HELPER(a, b) a##b

#define LIMES_CONCATENATE(item1, item2) LIMES_CONCATENATE_HELPER (item1, item2)


#define LIMES_NON_COPYABLE(ClassName)                 \
	ClassName (const ClassName&)			= delete; \
	ClassName& operator= (const ClassName&) = delete

#define LIMES_NON_MOVABLE(ClassName)             \
	ClassName (ClassName&&)			   = delete; \
	ClassName& operator= (ClassName&&) = delete


#define LIMES_DEFAULT_COPYABLE(ClassName)              \
	ClassName (const ClassName&)			= default; \
	ClassName& operator= (const ClassName&) = default

#define LIMES_DEFAULT_MOVABLE(ClassName)          \
	ClassName (ClassName&&)			   = default; \
	ClassName& operator= (ClassName&&) = default


#define LIMES_CONSTEXPR_COPYABLE(ClassName)                      \
	constexpr ClassName (const ClassName&)			  = default; \
	constexpr ClassName& operator= (const ClassName&) = default

#define LIMES_CONSTEXPR_MOVABLE(ClassName)                  \
	constexpr ClassName (ClassName&&)			 = default; \
	constexpr ClassName& operator= (ClassName&&) = default
