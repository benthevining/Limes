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

/** @defgroup preprocessor Preprocessor
	This module contains preprocessor macros defined by Limes.
	Limes attempts to keep its usage of macros to a minimum, but they are appropriate for some tasks in C++.
	@ingroup limes_core
 */

/** @file
	This file contains some utility preprocessor macros.
	@ingroup preprocessor

	@todo Fix documentation strings for the stub macros
 */


#pragma mark LIMES_HAS_INCLUDE

/* --- LIMES_HAS_INCLUDE --- */

#if defined(__has_include) || defined(DOXYGEN)

/** @def LIMES_HAS_INCLUDE
	A simple macro that always evaluates to false if the compiler doesn't support \c __has_include .
	@ingroup preprocessor
 */
#	define LIMES_HAS_INCLUDE(file) __has_include(file)
#else
#	define LIMES_HAS_INCLUDE(file) 0
#endif


#pragma mark LIMES_HAS_ATTRIBUTE

/* --- LIMES_HAS_ATTRIBUTE --- */

#if defined(__has_attribute) || defined(DOXYGEN)

/** @def LIMES_HAS_ATTRIBUTE
	A simple macro that always evaluates to false if the compiler doesn't support \c __has_attribute .
	@ingroup preprocessor
 */
#	define LIMES_HAS_ATTRIBUTE(attribute) __has_attribute (attribute)
#else
#	define LIMES_HAS_ATTRIBUTE(attribute) 0
#endif


#pragma mark LIMES_BLOCK_WITH_FORCED_SEMICOLON

/* --- LIMES_BLOCK_WITH_FORCED_SEMICOLON --- */

#if LIMES_MSVC && ! defined(DOXYGEN)
// clang-format off
#	define LIMES_BLOCK_WITH_FORCED_SEMICOLON(statement) \
		__pragma (warning (push))                       \
		__pragma (warning (disable : 4127))             \
		do {                                            \
			statement                                   \
		}  while (false)                                \
		__pragma (warning (pop))
// clang-format on
#else

/** @def LIMES_BLOCK_WITH_FORCED_SEMICOLON
	This macro can be used to wrap other macros, to force the user to write a semicolon after using it.
	For example:
	@code
	// this macro must be used with a semicolon after it
	#define MY_MACRO LIMES_BLOCK_WITH_FORCED_SEMICOLON(do_something();)

	MY_MACRO; // correct usage

	MY_MACRO  // compiler error
	@endcode
	@ingroup preprocessor
 */
// clang-format off
#	define LIMES_BLOCK_WITH_FORCED_SEMICOLON(statement) \
		do {                                            \
			statement                                   \
		} while (false)
// clang-format on
#endif


#pragma mark LIMES_MAKE_STRING

/* --- LIMES_MAKE_STRING --- */

/** @def LIMES_MAKE_STRING_HELPER
	For internal use by \c LIMES_MAKE_STRING.
	@see LIMES_MAKE_STRING
	@ingroup preprocessor
 */
#define LIMES_MAKE_STRING_HELPER(str) #str

/** @def LIMES_MAKE_STRING
	Turns any token into a quoted string.
	@ingroup preprocessor
 */
#define LIMES_MAKE_STRING(str) LIMES_MAKE_STRING_HELPER (str)


#pragma mark LIMES_CONCATENATE

/* --- LIMES_CONCATENATE --- */

/** @def LIMES_CONCATENATE_HELPER
	For internal use by \c LIMES_CONCATENATE.
	@see LIMES_CONCATENATE
	@ingroup preprocessor
 */
#define LIMES_CONCATENATE_HELPER(a, b) a##b

/** @def LIMES_CONCATENATE
	Concatenates any two tokens into a new token.
	@ingroup preprocessor
 */
#define LIMES_CONCATENATE(item1, item2) LIMES_CONCATENATE_HELPER (item1, item2)


#pragma mark Non-copyable/movable

/* --- Non-copyable/movable --- */

/** @def LIMES_NON_COPYABLE
	This macro can be placed into a class definition to delete its copy constructor and copy assignment operator.
	Example usage:
	@code
	struct MyStruct
	{
		LIMES_NON_COPYABLE(MyStruct)
	};
	@endcode
	@ingroup preprocessor
	@see LIMES_NON_MOVABLE, LIMES_DEFAULT_COPYABLE
 */
#define LIMES_NON_COPYABLE(ClassName)                 \
	/** @name Copying */                              \
	/** @{ */                                         \
	/** %##ClassName is not copyable. */              \
	ClassName (const ClassName&) = delete;            \
	/** %##ClassName is not copyable. */              \
	ClassName& operator= (const ClassName&) = delete; \
	///@}


/** @def LIMES_NON_MOVABLE
	This macro can be placed into a class definition to delete its move constructor and move assignment operator.
	Example usage:
	@code
	struct MyStruct
	{
		LIMES_NON_MOVABLE(MyStruct)
	};
	@endcode
	@ingroup preprocessor
	@see LIMES_NON_COPYABLE, LIMES_DEFAULT_MOVABLE
 */
#define LIMES_NON_MOVABLE(ClassName)             \
	/** @name Moving */                          \
	/** @{ */                                    \
	/** %##ClassName is not movable. */          \
	ClassName (ClassName&&) = delete;            \
	/** %##ClassName is not movable. */          \
	ClassName& operator= (ClassName&&) = delete; \
	///@}


#pragma mark Default copyable/movable

/* --- Default copyable/movable --- */

/** @def LIMES_DEFAULT_COPYABLE
	This macro can be placed into a class definition to default its copy constructor and copy assignment operator.
	Example usage:
	@code
	struct MyStruct
	{
		LIMES_DEFAULT_COPYABLE(MyStruct)
	};
	@endcode
	@ingroup preprocessor
	@see LIMES_CONSTEXPR_COPYABLE, LIMES_NON_COPYABLE, LIMES_DEFAULT_MOVABLE
 */
#define LIMES_DEFAULT_COPYABLE(ClassName)              \
	/** @name Copying */                               \
	/** @{ */                                          \
	/** %##ClassName is default copyable. */           \
	ClassName (const ClassName&) = default;            \
	/** %##ClassName is default copyable. */           \
	ClassName& operator= (const ClassName&) = default; \
	///@}


/** @def LIMES_DEFAULT_MOVABLE
	This macro can be placed into a class definition to default its move constructor and move assignment operator.
	Example usage:
	@code
	struct MyStruct
	{
		LIMES_DEFAULT_MOVABLE(MyStruct)
	};
	@endcode
	@ingroup preprocessor
	@see LIMES_CONSTEXPR_MOVABLE, LIMES_NON_MOVABLE, LIMES_DEFAULT_COPYABLE
 */
#define LIMES_DEFAULT_MOVABLE(ClassName)          \
	/** @name Moving */                           \
	/** @{ */                                     \
	/** %##ClassName is default movable. */       \
	ClassName (ClassName&&) = default;            \
	/** %##ClassName is default movable. */       \
	ClassName& operator= (ClassName&&) = default; \
	///@}


#pragma mark Constexpr copyable/movable

/* --- Constexpr copyable/movable --- */

/** @def LIMES_CONSTEXPR_COPYABLE
	Similar to \c LIMES_DEFAULT_COPYABLE, this macro can be placed into a class definition to default its copy constructor and copy assignment operator, but they will also be declared constexpr.
	Example usage:
	@code
	struct MyStruct
	{
		LIMES_CONSTEXPR_COPYABLE(MyStruct)
	};
	@endcode
	@see LIMES_DEFAULT_COPYABLE, LIMES_NON_COPYABLE, LIMES_CONSTEXPR_MOVABLE
	@ingroup preprocessor
 */
#define LIMES_CONSTEXPR_COPYABLE(ClassName)                      \
	/** @name Copying */                                         \
	/** @{ */                                                    \
	/** %##ClassName is default copyable at compile time. */     \
	constexpr ClassName (const ClassName&) = default;            \
	/** %##ClassName is default copyable at compile time. */     \
	constexpr ClassName& operator= (const ClassName&) = default; \
	///@}


/** @def LIMES_CONSTEXPR_MOVABLE
	Similar to \c LIMES_CONSTEXPR_COPYABLE, this macro can be placed into a class definition to default its move constructor and move assignment operator, but they will also be declared constexpr.
	Example usage:
	@code
	struct MyStruct
	{
		LIMES_CONSTEXPR_MOVABLE(MyStruct)
	};
	@endcode
	@see LIMES_DEFAULT_MOVABLE, LIMES_NON_MOVABLE, LIMES_CONSTEXPR_COPYABLE
	@ingroup preprocessor
 */
#define LIMES_CONSTEXPR_MOVABLE(ClassName)                           \
	/** @name Moving */                                              \
	/** @{ */                                                        \
	/** %##ClassName is default movable at compile time. */          \
	constexpr ClassName (ClassName&&) noexcept = default;            \
	/** %##ClassName is default movable at compile time. */          \
	constexpr ClassName& operator= (ClassName&&) noexcept = default; \
	///@}
