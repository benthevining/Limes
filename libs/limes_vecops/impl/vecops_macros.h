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

#include <limes_core.h>	 // for LIMES_COMPILER_WARNING
#include <limes_platform.h>

/** @file
	This file sets default values for any of the limes_vecops implementation-selecting
	macros that are not already defined, and checks that the final set of definitions is valid.

	This file defines the following macros:
	- LIMES_VECOPS_USE_VDSP
	- LIMES_VECOPS_USE_IPP
	- LIMES_VECOPS_USE_MIPP

	Only one of these will be defined to 1, the others will be defined to 0. All the macros will be
	defined to either 1 or 0, so you should check them using \c #if , not \c #ifdef .

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

#endif /* DOXYGEN */

/// @cond


/* --- LIMES_VECOPS_USE_VDSP --- */

#pragma mark LIMES_VECOPS_USE_VDSP

#ifdef LIMES_VECOPS_USE_VDSP

#	if LIMES_VECOPS_USE_VDSP
#		ifdef LIMES_VECOPS_USE_IPP
#			if LIMES_VECOPS_USE_IPP
LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_VDSP defined to 1, when LIMES_VECOPS_USE_IPP is also defined to 1!")
// if vdsp and ipp are both 1, prefer vdsp over ipp -- force define ipp to 0
#				undef LIMES_VECOPS_USE_IPP
#				define LIMES_VECOPS_USE_IPP 0
#			endif
#		endif

#		ifdef LIMES_VECOPS_USE_MIPP
#			if LIMES_VECOPS_USE_MIPP
LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_VDSP defined to 1, when LIMES_VECOPS_USE_MIPP is also defined to 1!")
// if vdsp and mipp are both 1, prefer vdsp over mipp -- force define mipp to 0
#				undef LIMES_VECOPS_USE_MIPP
#				define LIMES_VECOPS_USE_MIPP 0
#			endif
#		endif
#	endif

#else /* ifdef LIMES_VECOPS_USE_VDSP */

#	ifdef LIMES_VECOPS_USE_IPP
#		if LIMES_VECOPS_USE_IPP
#			define LIMES_VECOPS_USE_VDSP 0
#		endif
#	endif

#	ifndef LIMES_VECOPS_USE_VDSP
#		ifdef LIMES_VECOPS_USE_MIPP
#			if LIMES_VECOPS_USE_MIPP
#				define LIMES_VECOPS_USE_VDSP 0
#			endif
#		endif
#	endif

#	ifndef LIMES_VECOPS_USE_VDSP
#		if LIMES_APPLE && LIMES_HAS_INCLUDE(<Accelerate / Accelerate.h>)
#			define LIMES_VECOPS_USE_VDSP 1
#		else
#			define LIMES_VECOPS_USE_VDSP 0
#		endif
#	endif

#endif /* LIMES_VECOPS_USE_VDSP */

#ifndef LIMES_VECOPS_USE_VDSP
#	error Implementation error - LIMES_VECOPS_USE_VDSP not defined!
#endif


/* --- LIMES_VECOPS_USE_IPP --- */

#pragma mark LIMES_VECOPS_USE_IPP

#ifdef LIMES_VECOPS_USE_IPP

#	if LIMES_VECOPS_USE_IPP
#		if LIMES_VECOPS_USE_VDSP
LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_IPP defined to 1, when LIMES_VECOPS_USE_VDSP is also defined to 1!")
// if ipp and vdsp are both 1, prefer vdsp over ipp -- force define ipp to 0
#			undef LIMES_VECOPS_USE_IPP
#			define LIMES_VECOPS_USE_IPP 0
#		endif
#	endif

#	if LIMES_VECOPS_USE_IPP
#		ifdef LIMES_VECOPS_USE_MIPP
#			if LIMES_VECOPS_USE_MIPP
LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_IPP defined to 1, when LIMES_VECOPS_USE_MIPP is also defined to 1!")
// if ipp and mipp are both 1, prefer ipp over mipp -- force define mipp to 0
#				undef LIMES_VECOPS_USE_MIPP
#				define LIMES_VECOPS_USE_MIPP 0
#			endif
#		endif
#	endif

#else /* ifdef LIMES_VECOPS_USE_IPP */

#	if LIMES_VECOPS_USE_VDSP
#		define LIMES_VECOPS_USE_IPP 0
#	endif

#	ifndef LIMES_VECOPS_USE_IPP
#		ifdef LIMES_VECOPS_USE_MIPP
#			if LIMES_VECOPS_USE_MIPP
#				define LIMES_VECOPS_USE_IPP 0
#			endif
#		endif
#	endif

#	ifndef LIMES_VECOPS_USE_IPP
#		if LIMES_INTEL && LIMES_HAS_INCLUDE(<ipps.h>)
#			define LIMES_VECOPS_USE_IPP 1
#		else
#			define LIMES_VECOPS_USE_IPP 0
#		endif
#	endif

#endif /* LIMES_VECOPS_USE_IPP */

#ifndef LIMES_VECOPS_USE_IPP
#	error Implementation error - LIMES_VECOPS_USE_IPP not defined!
#endif


/* --- LIMES_VECOPS_USE_MIPP --- */

#pragma mark LIMES_VECOPS_USE_MIPP

#ifdef LIMES_VECOPS_USE_MIPP

#	if LIMES_VECOPS_USE_MIPP
#		if LIMES_VECOPS_USE_VDSP
LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_MIPP defined to 1, when LIMES_VECOPS_USE_VDSP is also defined to 1!")
// if mipp and vdsp are both 1, prefer vdsp over mipp -- force define mipp to 0
#			undef LIMES_VECOPS_USE_MIPP
#			define LIMES_VECOPS_USE_MIPP 0
#		endif
#	endif

#	if LIMES_VECOPS_USE_MIPP
#		if LIMES_VECOPS_USE_IPP
LIMES_COMPILER_WARNING ("LIMES_VECOPS_USE_MIPP defined to 1, when LIMES_VECOPS_USE_IPP is also defined to 1!")
// if mipp and ipp are both 1, prefer ipp over mipp -- force define mipp to 0
#			undef LIMES_VECOPS_USE_MIPP
#			define LIMES_VECOPS_USE_MIPP 0
#		endif
#	endif

#else /* ifdef LIMES_VECOPS_USE_MIPP */

#	if LIMES_VECOPS_USE_VDSP
#		define LIMES_VECOPS_USE_MIPP 0
#	endif

#	ifndef LIMES_VECOPS_USE_MIPP
#		if LIMES_VECOPS_USE_IPP
#			define LIMES_VECOPS_USE_MIPP 0
#		endif
#	endif

#	ifndef LIMES_VECOPS_USE_MIPP
#		if (LIMES_SSE || LIMES_ARM_NEON || LIMES_AVX || LIMES_AVX512) && LIMES_HAS_INCLUDE(<mipp.h>)
#			define LIMES_VECOPS_USE_MIPP 1
#		else
#			define LIMES_VECOPS_USE_MIPP 0
#		endif
#	endif

#endif /* LIMES_VECOPS_USE_MIPP */

#ifndef LIMES_VECOPS_USE_MIPP
#	error Implementation error - LIMES_VECOPS_USE_MIPP not defined!
#endif


/* --- wrapup --- */

#pragma mark Error checking

#if ((LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_IPP)     \
	 || (LIMES_VECOPS_USE_VDSP && LIMES_VECOPS_USE_MIPP) \
	 || (LIMES_VECOPS_USE_IPP && LIMES_VECOPS_USE_MIPP))
#	error Only one of LIMES_VECOPS_USE_VDSP, LIMES_VECOPS_USE_IPP, or LIMES_VECOPS_USE_MIPP may be set to 1!
#endif

#if ! (LIMES_VECOPS_USE_VDSP || LIMES_VECOPS_USE_IPP || LIMES_VECOPS_USE_MIPP)
#	error One of LIMES_VECOPS_USE_VDSP, LIMES_VECOPS_USE_IPP, or LIMES_VECOPS_USE_MIPP must be set to 1!
#endif

/// @endcond
