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
	This file defines macros for disabling compiler warnings, and emitting custom compiler warnings.
	@ingroup preprocessor
 */

#ifdef DOXYGEN

/** @def LIMES_DISABLE_ALL_COMPILER_WARNINGS
	This macro disables all compiler warnings from the point in the source code where it is encountered until \c LIMES_REENABLE_ALL_COMPILER_WARNINGS is encountered.
	This is useful for including external headers that may cause lots of warnings.
	Example usage:
	@code
	LIMES_DISABLE_ALL_COMPILER_WARNINGS

	#include <someExternalHeader>

	LIMES_REENABLE_ALL_COMPILER_WARNINGS
	@endcode
	@see LIMES_REENABLE_ALL_COMPILER_WARNINGS
	@ingroup preprocessor
 */
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS

/** @def LIMES_REENABLE_ALL_COMPILER_WARNINGS
	This macro re-enables all compiler warnings after \c LIMES_DISABLE_ALL_COMPILER_WARNINGS has been issued.
	Example usage:
	@code
	LIMES_DISABLE_ALL_COMPILER_WARNINGS

	#include <someExternalHeader>

	LIMES_REENABLE_ALL_COMPILER_WARNINGS
	@endcode
	@see LIMES_DISABLE_ALL_COMPILER_WARNINGS
	@ingroup preprocessor
 */
#	define LIMES_REENABLE_ALL_COMPILER_WARNINGS

/** @def LIMES_COMPILER_MESSAGE
	Displays a custom message with the compiler's output. To display a custom warning message, use \c LIMES_COMPILER_WARNING.
	Some compilers don't support custom messages, so the message may be emitted as a warning.
	@see LIMES_COMPILER_WARNING
	@ingroup preprocessor
 */
#	define LIMES_COMPILER_MESSAGE(text)

/** @def LIMES_COMPILER_WARNING
	This macro causes the compiler to print the given text as a compilation warning, if available with the currently used compiler.
	Some compilers don't support user-defined warnings, so the text may just be printed to std::out during compilation with no actual warning issued.
	@see LIMES_COMPILER_MESSAGE
	@ingroup preprocessor
 */
#	define LIMES_COMPILER_WARNING(text)

#endif /* DOXYGEN */

/// @cond

#if LIMES_CLANG || LIMES_GCC
#	define LIMES_COMPILER_WARNING(text) \
		_Pragma (LIMES_MAKE_STRING (GCC warning (text)))
#endif

#if LIMES_GCC || LIMES_CLANG || LIMES_MSVC || LIMES_INTEL_COMPILER
#	define LIMES_COMPILER_MESSAGE(text) \
		_Pragma (LIMES_MAKE_STRING (message text))
#elif LIMES_CRAY_COMPILER
#	define LIMES_COMPILER_MESSAGE(text) \
		_Pragma (LIMES_MAKE_STRING (_CRI message text))
#endif


#if LIMES_CLANG

// clang-format off
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("clang diagnostic push")      \
		_Pragma ("clang diagnostic ignored \"-Weverything\"")
// clang-format on

#	define LIMES_REENABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("clang diagnostic pop")

#elif LIMES_GCC

// clang-format off
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS								   \
		_Pragma ("GCC diagnostic push")									       \
		_Pragma ("GCC diagnostic ignored \"-Wall\"")						   \
		_Pragma ("GCC diagnostic ignored \"-Wpragmas\"")					   \
		_Pragma ("GCC diagnostic ignored \"-Wextra\"")						   \
		_Pragma ("GCC diagnostic ignored \"-Wshadow\"")						   \
		_Pragma ("GCC diagnostic ignored \"-Wunused-parameter\"")			   \
		_Pragma ("GCC diagnostic ignored \"-Wconversion\"")					   \
		_Pragma ("GCC diagnostic ignored \"-Wsign-conversion\"")			   \
		_Pragma ("GCC diagnostic ignored \"-Wswitch-enum\"")				   \
		_Pragma ("GCC diagnostic ignored \"-Wzero-as-null-pointer-constant\"") \
		_Pragma ("GCC diagnostic ignored \"-Wunused-variable\"")			   \
		_Pragma ("GCC diagnostic ignored \"-Wredundant-decls\"")			   \
		_Pragma ("GCC diagnostic ignored \"-Wswitch-enum\"")				   \
		_Pragma ("GCC diagnostic ignored \"-Wsubobject-linkage\"")			   \
		_Pragma ("GCC diagnostic ignored \"-Wunused-but-set-variable\"")	   \
		_Pragma ("GCC diagnostic ignored \"-Wredundant-move\"")				   \
		_Pragma ("GCC diagnostic ignored \"-Wstrict-aliasing\"")			   \
		_Pragma ("GCC diagnostic ignored \"-Woverloaded-virtual\"")			   \
		_Pragma ("GCC diagnostic ignored \"-Wnoexcept\"")					   \
		_Pragma ("GCC diagnostic ignored \"-Wc99-extensions\"")				   \
// clang-format on

#	define LIMES_REENABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("GCC diagnostic pop")

#elif (LIMES_MSVC || LIMES_INTEL_COMPILER)

// clang-format off
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("warning (push, 0)")          \
		_Pragma ("warning (disable : 4702)")
// clang-format on

#	define LIMES_REENABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("warning (pop)")

#endif

#ifndef LIMES_DISABLE_ALL_COMPILER_WARNINGS
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS
#endif

#ifndef LIMES_REENABLE_ALL_COMPILER_WARNINGS
#	define LIMES_REENABLE_ALL_COMPILER_WARNINGS
#endif

#ifndef LIMES_COMPILER_MESSAGE
#	ifdef LIMES_COMPILER_WARNING
#		define LIMES_COMPILER_MESSAGE(text) LIMES_COMPILER_WARNING (text)
#	else
#		define LIMES_COMPILER_MESSAGE(text)
#	endif
#endif

#ifndef LIMES_COMPILER_WARNING
#	define LIMES_COMPILER_WARNING(text) LIMES_COMPILER_MESSAGE (text)
#endif

/// @endcond
