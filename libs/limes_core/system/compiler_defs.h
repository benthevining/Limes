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

#ifdef DOXYGEN

#	define LIMES_FORCE_INLINE
#	define LIMES_NEVER_INLINE
#	define LIMES_PURE_FUNCTION
#	define LIMES_RESTRICT

#endif

/// @cond

#if LIMES_MSVC
#	define LIMES_FORCE_INLINE __forceinline
#	define LIMES_NEVER_INLINE __declspec(noinline)
#	define LIMES_RESTRICT	   __restrict

#else

#	if LIMES_HAS_ATTRIBUTE(pure)
#		define LIMES_PURE_FUNCTION __attribute__ ((pure))
#	elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#		define LIMES_PURE_FUNCTION _Pragma ("does_not_write_global_data")
#	elif LIMES_ARM_COMPILER
#		define LIMES_PURE_FUNCTION __pure
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

#endif

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

/// @endcond
