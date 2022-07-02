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

// IWYU pragma: private
// IWYU pragma: friend limes_vecops.h

#pragma once

#include <limes_core.h>	 // for LIMES_COMPILER_WARNING, platform defs

/** @file
	This file sets default values for any of the limes_vecops implementation-selecting
	macros that are not already defined, and checks that the final set of definitions is valid.

	@ingroup limes_vecops
 */

#pragma mark Doxygen docs

#ifdef DOXYGEN

/** @def LIMES_VECOPS_USE_VDSP
	1 if the Apple vDSP library is being used, otherwise 0.
	If this is undefined, then if neither \c LIMES_VECOPS_USE_IPP or \c LIMES_VECOPS_USE_MIPP are defined,
	the default is to define \c LIMES_VECOPS_USE_VDSP to 1 if on an Apple platform, and 0 otherwise.
	If either \c LIMES_VECOPS_USE_IPP or \c LIMES_VECOPS_USE_MIPP are defined, \c LIMES_VECOPS_USE_VDSP will be defined to 0
	and a warning will be issued if it was defined to 1.
	@ingroup limes_vecops
	@see LIMES_VECOPS_USE_IPP, LIMES_VECOPS_USE_MIPP
 */
#	define LIMES_VECOPS_USE_VDSP 0

/** @def LIMES_VECOPS_USE_IPP
	1 if the Intel IPP library is being used, otherwise 0.
	If this is undefined, then if neither \c LIMES_VECOPS_USE_VDSP or \c LIMES_VECOPS_USE_MIPP are defined,
	the default is to define \c LIMES_VECOPS_USE_IPP to 1 if on an Intel platform and the IPP header can be included, and 0 otherwise.
	If either \c LIMES_VECOPS_USE_VDSP or \c LIMES_VECOPS_USE_MIPP are defined, \c LIMES_VECOPS_USE_IPP will be defined to 0
	and a warning will be issued if it was defined to 1.
	@ingroup limes_vecops
	@see LIMES_VECOPS_USE_VDSP, LIMES_VECOPS_USE_MIPP
 */
#	define LIMES_VECOPS_USE_IPP 0

/** @def LIMES_VECOPS_USE_MIPP
	1 if the MIPP library is being used, otherwise 0.
	If this is undefined, then if neither \c LIMES_VECOPS_USE_VDSP or \c LIMES_VECOPS_USE_IPP are defined,
	the default is to define \c LIMES_VECOPS_USE_MIPP to 1 if on an Intel platform and the IPP header can be included, and 0 otherwise.
	If either \c LIMES_VECOPS_USE_VDSP or \c LIMES_VECOPS_USE_IPP are defined, \c LIMES_VECOPS_USE_MIPP will be defined to 0
	and a warning will be issued if it was defined to 1.
	@ingroup limes_vecops
	@see LIMES_VECOPS_USE_VDSP, LIMES_VECOPS_USE_IPP
 */
#	define LIMES_VECOPS_USE_MIPP 0

/**	@def LIMES_VECOPS_USE_POMMIER
	1 if the Pommier SIMD extensions are being used, otherwise 0.
	See neon_mathfun.h for the original license that accompanies these functions.
	@ingroup limes_vecops
 */
#	define LIMES_VECOPS_USE_POMMIER 0

#endif /* DOXYGEN */

/// @cond

#pragma mark LIMES_VECOPS_USE_VDSP

/* --- LIMES_VECOPS_USE_VDSP --- */

LIMES_DISABLE_ALL_COMPILER_WARNINGS
#ifndef LIMES_VECOPS_USE_VDSP
#	if (LIMES_VECOPS_USE_IPP || LIMES_VECOPS_USE_MIPP)
#		define LIMES_VECOPS_USE_VDSP 0
#	elif LIMES_APPLE
#		define LIMES_VECOPS_USE_VDSP 1
#	else
#		define LIMES_VECOPS_USE_VDSP 0
#	endif
#endif
LIMES_REENABLE_ALL_COMPILER_WARNINGS

#pragma mark LIMES_VECOPS_USE_IPP

/* --- LIMES_VECOPS_USE_IPP --- */

// clang-format off
#if LIMES_VECOPS_USE_VDSP
#	if LIMES_VECOPS_USE_IPP
	LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_IPP evaluated to true, when LIMES_VECOPS_USE_VDSP was already on!")
#	endif
// clang-format on

#	undef LIMES_VECOPS_USE_IPP
#	define LIMES_VECOPS_USE_IPP 0
#endif

LIMES_DISABLE_ALL_COMPILER_WARNINGS
#ifndef LIMES_VECOPS_USE_IPP
#	if (LIMES_VECOPS_USE_VDSP || LIMES_VECOPS_USE_MIPP)
#		define LIMES_VECOPS_USE_IPP 0
#	elif LIMES_INTEL && LIMES_HAS_INCLUDE(<ipps.h>)
#		define LIMES_VECOPS_USE_IPP 1
#	else
#		define LIMES_VECOPS_USE_IPP 0
#	endif
#endif
LIMES_REENABLE_ALL_COMPILER_WARNINGS

#pragma mark LIMES_VECOPS_USE_MIPP

/* --- LIMES_VECOPS_USE_MIPP --- */

// clang-format off
#if (LIMES_VECOPS_USE_VDSP || LIMES_VECOPS_USE_IPP)
#	if LIMES_VECOPS_USE_MIPP
	LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_MIPP evaluated to true, when LIMES_VECOPS_USE_VDSP or LIMES_VECOPS_USE_IPP was already on!")
#	endif
// clang-format on

#	undef LIMES_VECOPS_USE_MIPP
#	define LIMES_VECOPS_USE_MIPP 0
#endif

LIMES_DISABLE_ALL_COMPILER_WARNINGS
#ifndef LIMES_VECOPS_USE_MIPP
#	if (LIMES_VECOPS_USE_VDSP || LIMES_VECOPS_USE_IPP)
#		define LIMES_VECOPS_USE_MIPP 0
#	elif (LIMES_SSE || LIMES_AVX || LIMES_AVX512 || LIMES_ARM_NEON) && LIMES_HAS_INCLUDE(<mipp.h>)
#		define LIMES_VECOPS_USE_MIPP 1
#	else
#		define LIMES_VECOPS_USE_MIPP 0
#	endif
#endif
LIMES_REENABLE_ALL_COMPILER_WARNINGS

#pragma mark Error checking

/* --- wrapup --- */

#if ((LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_IPP)     \
	 || (LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_MIPP) \
	 || (LIMES_VECOPS_USE_IPP && LIMES_VECOPS_USE_MIPP))
#	error "Only one of LIMES_VECOPS_USE_VDSP, LIMES_VECOPS_USE_IPP, or LIMES_VECOPS_USE_MIPP may be set to 1!"
#endif

// clang-format off
#if LIMES_VECOPS_USE_VDSP
	LIMES_COMPILER_MESSAGE ("limes_vecops: using Apple vDSP backend")
#elif LIMES_VECOPS_USE_IPP
	LIMES_COMPILER_MESSAGE ("limes_vecops: using Intel IPP backend")
#elif LIMES_VECOPS_USE_MIPP
	LIMES_COMPILER_MESSAGE ("limes_vecops: using MIPP backend")
#else
	LIMES_COMPILER_MESSAGE ("limes_vecops: using fallback backend")
#endif
// clang-format on

#pragma mark LIMES_VECOPS_USE_POMMIER

/* --- LIMES_VECOPS_USE_POMMIER --- */

// clang-format off
#ifdef LIMES_VECOPS_USE_POMMIER
#	if LIMES_VECOPS_USE_POMMIER
#		if ! (LIMES_ARM_NEON || LIMES_SSE)
#			error Pommier extensions cannot be used if neither NEON nor SSE is available!
#		endif
#	else
#		if (LIMES_ARM_NEON || LIMES_SSE)
			LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_POMMIER is off, but NEON or SSE are available!")
#		endif
#	endif
#endif
// clang-format on

#ifndef LIMES_VECOPS_USE_POMMIER
#	if (LIMES_ARM_NEON || (LIMES_SSE && ! LIMES_MSVC))	 // MSVC doesn't like to compile MMX intrinsics
#		define LIMES_VECOPS_USE_POMMIER 1
#	else
#		define LIMES_VECOPS_USE_POMMIER 0
#	endif
#endif

// clang-format off
#if LIMES_VECOPS_USE_POMMIER
	LIMES_COMPILER_MESSAGE ("limes_vecops: using Pommier SIMD functions")
#endif
// clang-format on

/// @endcond
