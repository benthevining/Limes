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

#ifdef DOXYGEN

/** This macro disables all compiler warnings from the point in the source code where it is encountered until \c LIMES_REENABLE_ALL_COMPILER_WARNINGS is encountered.
	@see LIMES_REENABLE_ALL_COMPILER_WARNINGS
 */
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS

/** This macro re-enables all compiler warnings after \c LIMES_DISABLE_ALL_COMPILER_WARNINGS has been issued.
	@see LIMES_DISABLE_ALL_COMPILER_WARNINGS
 */
#	define LIMES_REENABLE_ALL_COMPILER_WARNINGS

#else

#	if LIMES_CLANG

// clang-format off
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("clang diagnostic push")      \
		_Pragma ("clang diagnostic ignored \"-Weverything\"")
// clang-format on

#		define LIMES_REENABLE_ALL_COMPILER_WARNINGS \
			_Pragma ("clang diagnostic pop")

#	elif LIMES_GCC

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
		_Pragma ("GCC diagnostic ignored \"-Wc99-extensions\"")				   \
// clang-format on

#	define LIMES_REENABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("GCC diagnostic pop")

#elif LIMES_MSVC

// clang-format off
#	define LIMES_DISABLE_ALL_COMPILER_WARNINGS \
		_Pragma ("warning (push, 0)")          \
		_Pragma ("warning (disable : 4702)")
// clang-format on

#		define LIMES_REENABLE_ALL_COMPILER_WARNINGS \
			_Pragma ("warning (pop)")

#	else
#		define LIMES_DISABLE_ALL_COMPILER_WARNINGS
#		define LIMES_REENABLE_ALL_COMPILER_WARNINGS
#	endif

#endif
