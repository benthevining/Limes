/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once
#include <cassert>

/*
	These conditionals declare the macros
	  - LIMES_WINDOWS
	  - LIMES_ANDROID
	  - LIMES_LINUX
	  - LIMES_APPLE
	  - LIMES_OSX
	  - LIMES_IOS
	  - LIMES_BSD
	  - LIMES_POSIX
	...based on the current operating system.
	It also declares a string literal macro LIMES_OPERATING_SYSTEM_NAME which can be used if you need a text description of the OS.
*/
#if defined(_WIN32) || defined(_WIN64)
#	define LIMES_WINDOWS				1
#	define LIMES_OPERATING_SYSTEM_NAME "Windows"
#elif __ANDROID__
#	define LIMES_ANDROID				1
#	define LIMES_OPERATING_SYSTEM_NAME "Android"
#elif defined(LINUX) || defined(__linux__)
#	define LIMES_LINUX					1
#	define LIMES_OPERATING_SYSTEM_NAME "Linux"
#elif __APPLE__
#	define LIMES_APPLE 1
#	include <TargetConditionals.h>
#
#	if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#		define LIMES_IOS					1
#		define LIMES_OSX					0
#		define LIMES_OPERATING_SYSTEM_NAME "iOS"
#	else
#		define LIMES_OSX					1
#		define LIMES_IOS					0
#		define LIMES_OPERATING_SYSTEM_NAME "MacOS"
#	endif
#elif defined(__FreeBSD__) || (__OpenBSD__)
#	define LIMES_BSD					1
#	define LIMES_OPERATING_SYSTEM_NAME "BSD"
#elif defined(_POSIX_VERSION)
#	define LIMES_POSIX					1
#	define LIMES_OPERATING_SYSTEM_NAME "Posix"
#else
#	error Unknown platform!
#endif

#ifndef LIMES_WINDOWS
#	define LIMES_WINDOWS 0
#endif

#ifndef LIMES_ANDROID
#	define LIMES_ANDROID 0
#endif

#ifndef LIMES_LINUX
#	define LIMES_LINUX 0
#endif

#ifndef LIMES_APPLE
#	define LIMES_APPLE 0
#	define LIMES_IOS	0
#	define LIMES_OSX0
#endif

#ifndef LIMES_BSD
#	define LIMES_BSD 0
#endif

#ifndef LIMES_POSIX
#	define LIMES_POSIX 0
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/

/** Compiler type macros. */

#if defined(__clang__)
#	define LIMES_CLANG	   1
#	define LIMES_COMPILER "Clang"
#elif defined(__GNUC__)
#	define LIMES_GCC	   1
#	define LIMES_COMPILER "GCC"
#elif defined(_MSC_VER)
#	define LIMES_MSVC	   1
#	define LIMES_COMPILER "MSVC"
#else
#	error Unknown compiler!
#endif

#ifndef LIMES_CLANG
#	define LIMES_CLANG 0
#endif

#ifndef LIMES_GCC
#	define LIMES_GCC 0
#endif

#ifndef LIMES_MSVC
#	define LIMES_MSVC 0
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/

#if LIMES_WINDOWS

#	ifdef _DEBUG
#		define LIMES_DEBUG 1
#	endif

#	define LIMES_INTEL 1

#elif LIMES_APPLE

#	if defined(DEBUG) || defined(_DEBUG) || ! (defined(NDEBUG) || defined(_NDEBUG))
#		define LIMES_DEBUG 1
#	endif

#	if defined(__arm__) || defined(__arm64__)
#		define LIMES_ARM 1
#	else
#		define LIMES_INTEL 1
#	endif

#elif (LIMES_LINUX || LIMES_ANDROID || LIMES_BSD)

#	ifdef _DEBUG
#		define LIMES_DEBUG 1
#	endif

#	if defined(__arm__) || defined(__arm64__) || defined(__aarch64__)
#		define LIMES_ARM 1
#	elif __MMX__ || __SSE__ || __amd64__
#		define LIMES_INTEL 1
#	endif

#endif

#ifndef LIMES_DEBUG
#	define LIMES_DEBUG 0
#endif

#if LIMES_DEBUG
#	define LIMES_BUILD_MODE "Debug"
#else
#	define LIMES_BUILD_MODE "Release"
#endif

#ifndef LIMES_INTEL
#	define LIMES_INTEL 0
#endif

#ifndef LIMES_ARM
#	define LIMES_ARM 0
#endif

#if LIMES_INTEL
#	define LIMES_PROCESSOR "Intel"
#elif LIMES_ARM
#	define LIMES_PROCESSOR "ARM"
#else
#	define LIMES_PROCESSOR "Unknown"
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/

#ifndef LIMES_ASSERT_FALSE
#	ifdef jassertfalse
#		define LIMES_ASSERT_FALSE jassertfalse
#	else
#		define LIMES_ASSERT_FALSE assert (false)
#	endif
#endif

#ifndef LIMES_ASSERT
#	ifdef jassert
#		define LIMES_ASSERT(x) jassert (x)
#	else
#		define LIMES_ASSERT(x) assert (x)
#	endif
#endif
