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
#include "../misc/preprocessor.h"

/** @file
	This file defines various compiler-specific utility macros.
	@ingroup system
 */

#ifdef DOXYGEN

/** @def LIMES_CURRENT_FUNCTION_NAME
	A string-literal that expands to the current function name, pretty-printed if supported by the compiler.
 */
#	define LIMES_CURRENT_FUNCTION_NAME

/** @def LIMES_FORCE_INLINE
	Forces a function to always be inlined.
	Example usage:
	@code
	LIMES_FORCE_INLINE void someFunction();
	@endcode
	@ingroup system
 */
#	define LIMES_FORCE_INLINE

/** @def LIMES_NEVER_INLINE
	Forces a function to never be inlined.
	Example usage:
	@code
	LIMES_NEVER_INLINE void someFunction();
	@endcode
	@ingroup system
 */
#	define LIMES_NEVER_INLINE

/** @def LIMES_PURE_FUNCTION
	Specifies that a function is a pure function; ie, that it has no side effects.
	Example usage:
	@code
	LIMES_PURE_FUNCTION void someFunction();
	@endcode
	@ingroup system
 */
#	define LIMES_PURE_FUNCTION

/** @def LIMES_RESTRICT
	Pointer anti-aliasing macro.
	Example usage:
	@code
	void someFunction (char* LIMES_RESTRICT ptrA, char* LIMES_RESTRICT ptrB);
	@endcode
	@ingroup system
 */
#	define LIMES_RESTRICT

/** @def LIMES_PACK
	This macro can be used to wrap an entire class declaration to inform the compiler that its memory layout should be packed as tightly as possible.
	Example usage:
	@code
	LIMES_PACK(
	struct PackedStruct
	{
		...
	};
	)
	@endcode
	@ingroup system
 */
#	define LIMES_PACK(declaration)

#endif /* DOXYGEN */

/// @cond

#if LIMES_CLANG || defined(__GNUC__)
#	define LIMES_CURRENT_FUNCTION_NAME __PRETTY_FUNCTION__
#elif LIMES_MSVC
#	define LIMES_CURRENT_FUNCTION_NAME __FUNCSIG__
#elif defined(__SUNPRO_CC)
#	define LIMES_CURRENT_FUNCTION_NAME __func__
#else
#	define LIMES_CURRENT_FUNCTION_NAME __FUNCTION__
#endif

#if LIMES_MSVC
#	define LIMES_FORCE_INLINE __forceinline
#	define LIMES_NEVER_INLINE __declspec(noinline)
#	define LIMES_RESTRICT	   __restrict

#	define LIMES_PACK(declaration) \
		__pragma (pack (push, 1))   \
			declaration             \
			__pragma (pack (pop))

#else /* MSVC */

#	if LIMES_HAS_ATTRIBUTE(pure)
#		define LIMES_PURE_FUNCTION __attribute__ ((pure))
#	elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#		define LIMES_PURE_FUNCTION _Pragma ("does_not_write_global_data")
#	elif LIMES_ARM_COMPILER
#		define LIMES_PURE_FUNCTION __pure
#	endif

#	if LIMES_HAS_ATTRIBUTE(__packed__)
#		define LIMES_PACK(declaration) declaration __attribute__ ((__packed__))
#	endif

#	if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#		define LIMES_RESTRICT _Restrict
#	elif ! LIMES_CRAY_COMPILER
#		define LIMES_RESTRICT __restrict
#	endif

#	if LIMES_INTEL_COMPILER
#		define LIMES_FORCE_INLINE _Pragma ("forceinline recursive") inline
#	elif LIMES_HAS_ATTRIBUTE(always_inline)
#		define LIMES_FORCE_INLINE inline __attribute__ ((always_inline))
#	elif LIMES_ARM_COMPILER
#		define LIMES_FORCE_INLINE __forceinline
#	elif LIMES_CRAY_COMPILER
#		define LIMES_FORCE_INLINE _Pragma ("_CRI inline_always") inline
#	endif

#	if LIMES_HAS_ATTRIBUTE(__noinline__)
#		define LIMES_NEVER_INLINE __attribute__ ((__noinline__))
#	elif LIMES_INTEL_COMPILER
#		define LIMES_NEVER_INLINE _Pragma ("noinline")
#	elif LIMES_CRAY_COMPILER
#		define LIMES_NEVER_INLINE _Pragma ("_CRI inline_never")
#	endif

#endif /* MSVC */


#ifndef LIMES_FORCE_INLINE
#	define LIMES_FORCE_INLINE inline
#endif

#ifndef LIMES_NEVER_INLINE
#	define LIMES_NEVER_INLINE
#endif

#ifndef LIMES_RESTRICT
#	define LIMES_RESTRICT
#endif

#ifndef LIMES_PURE_FUNCTION
#	define LIMES_PURE_FUNCTION
#endif

#ifndef LIMES_PACK
#	define LIMES_PACK(declaration) declaration
#endif

/// @endcond
