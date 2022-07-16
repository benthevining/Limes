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

/** @defgroup limes_vecops limes_vecops
	The Limes vector operations library.

	@anchor lib_limes_vecops

	All classes and functions in this module are accessible after linking to the \c limes_vecops library and including limes_vecops.h.

	This library provides functions operating on vectors of scalar data, accelerated with SIMD where possible.

	Because many vendor- or platform-specific libraries exist for such tasks, this library aims to provide a common API that can be implemented with any such 'backend'.
	This library currently supports using Apple vDSP, Intel IPP, or the MIPP library as backends, though more may be added in the future.

	This library also provides plain C++ implementations of all its functions, so it can be used without dependencies on any external libraries.
	Additionally, these fallback implementations can optionally call Julien Pommier's SIMD sin and cos functions, written for both NEON and SSE, if the datatype is \c float .
	These functions can be explicitly enabled or disabled using the \c LIMES_VECOPS_USE_POMMIER preprocessor macro.
	The Pommier source code is included in Limes, but using it requires your adherence to its original license; see \c neon_mathfun.h for a copy of it.

	@par "Limes library dependencies"
	@ref lib_limes_core "limes_core"

	One of the following libraries may be used as the backend for the vector operations functions:

	@dependency \b Accelerate
	<a href="https://developer.apple.com/documentation/accelerate?language=objc">Apple's Accelerate framework</a> can be used as the backend for the
	@ref limes_vecops "limes_vecops" functions. Accelerate comes with MacOS; it should work out of the box.
	To tell Limes to prefer Accelerate over other backends when available, set the \c LIMES_USE_VDSP @ref cmakeopt "CMake variable" to \c ON .

	@dependency \b IPP
	<a href="https://www.intel.com/content/www/us/en/developer/tools/oneapi/ipp.html#gs.452k3t">Intel's IPP (Integrated Performance Primitives) framework</a>
	can be used as the backend for the @ref limes_vecops "limes_vecops" functions. IPP must be manually installed to the system by the developer using Intel's
	installer program. To tell Limes to prefer IPP over other backends when available, set the \c LIMES_USE_IPP @ref cmakeopt "CMake variable" to \c ON .

	@dependency \b MIPP
	<a href="https://github.com/aff3ct/MIPP">The MIPP library</a> can be used as the backend for the @ref limes_vecops "limes_vecops" functions.
	MIPP is a library wrapping various SIMD intrinsics; it currently supports AVX, AVX-512, ARM NEON, and SSE.
	If MIPP is used, its sources will be fetched from GitHub at configure time if necessary.
	To tell Limes to prefer MIPP over other backends when available, set the \c LIMES_USE_MIPP @ref cmakeopt "CMake variable" to \c ON .

	@dependency \b NE10
	<a href="https://github.com/projectNe10/Ne10">The NE10 library</a> can be used as the backend for the @ref limes_vecops "limes_vecops" functions.
	NE10 is a library heavily optimized for ARM, but only offering functions for float data.
	If NE10 is used, fallback implementations for functions with other datatypes will be used.
	To tell Limes to prefer NE10 over other backends when available, set the \c LIMES_USE_NE10 @ref cmakeopt "CMake variable" to \c ON .

	If a SIMD library is not used, then "pure C++" implementations of all the functions defined in this file will be used.

	Here is an overview of the SIMD libraries that can be used as backends for this library's functions:

	Name                                                                                                                              | Vendor      | Preprocessor macro            | CMake variable            | Notes
	--------------------------------------------------------------------------------------------------------------------------------- | ----------- | ----------------------------- | ------------------------- | ----------------------------------------------
	<a href="https://developer.apple.com/documentation/accelerate?language=objc">vDSP (Accelerate)</a>                                | Apple       | LIMES_VECOPS_USE_VDSP         | LIMES_USE_VDSP            | The best choice on Apple platforms
	<a href="https://www.intel.com/content/www/us/en/developer/tools/oneapi/ipp.html#gs.452k3t">Integrated Performance Primitives</a> | Intel       | LIMES_VECOPS_USE_IPP          | LIMES_USE_IPP             | The best choice on Intel platforms
	<a href="https://github.com/aff3ct/MIPP">MyIntrinsics++ (MIPP)</a>                                                                | Open source | LIMES_VECOPS_USE_MIPP         | LIMES_USE_MIPP            | Supports NEON, SSE, AVX and AVX-512
	<a href="https://github.com/projectNe10/Ne10">NE10</a>                                                                            | ARM         | LIMES_VECOPS_USE_NE10         | LIMES_USE_NE10            | Optimized for ARM, but only works with floats
	Fallback (non-SIMD code)                                                                                                          | Limes       | None - absence of the other 3 | LIMES_USE_VECOPS_FALLBACK | Non-SIMD C++ implementations

	The default selection of the backend follows these heuristics.
	Each condition is evaluated in this order, choosing the first library to meet any of these conditions:
	- Use vDSP if on an Apple platform
	- Use IPP if on an Intel platform and it can be found on the system
	- Use MIPP if on a platform that supports NEON, SSE, AVX, or AVX512 (fetching the sources from GitHub if necessary)
	- Use NE10 if on an ARM platform
	- Otherwise, use the builtin fallback implementations

	However, these defaults can be overridden using the following CMake options:

	@cmakeopt \b LIMES_USE_VDSP If \c ON , prefer to use Apple vDSP over any other backend.
	@cmakeopt \b LIMES_USE_IPP If \c ON , prefer to use Intel IPP over any other backend.
	@cmakeopt \b LIMES_USE_MIPP If \c ON , prefer to use MIPP over any other backend.
	@cmakeopt \b LIMES_USE_NE10 If \c ON , prefer to use NE10 over any other backend.
	@cmakeopt \b LIMES_USE_VECOPS_FALLBACK If \c ON , prefer to use the builtin fallbacks over any third party backend.

	@cmakeprop \b LIMES_VECOPS_IMPLEMENTATION
	String name of the vector operations backend being used for the @ref lib_limes_vecops "limes_vecops" library.

	@cmaketarget \b Limes::limes_vecops
	The @ref lib_limes_vecops "limes_vecops"
	library target.

	@installcomp \b limes_vecops_runtime
	Runtime install component for the @ref lib_limes_vecops "limes_vecops"
	library.

	@installcomp \b limes_vecops_dev
	@parblock
	Development install component for the @ref lib_limes_vecops "limes_vecops"
	library. Depends on the \c limes_vecops_runtime component.

	This component is analogous to the `find_package()` component \c Vecops .
	@endparblock

	@installcompgroup \b limes_vecops
	This group includes the \c limes_vecops_dev and \c limes_vecops_runtime components.

	@ingroup limes

	@todo normalize function
	@todo type conversion functions
	@todo convert & copy functions
	@todo tone generation functions
	@todo filters
	@todo convolution engine
	@todo write unit tests

	@todo Intel MKL backend
	@todo ARM performance libraries backend
	@todo CUDA backend
 */

/** @dir libs/limes_vecops
	This directory contains the limes_vecops library.
	@ingroup limes_vecops
 */

/** @file
	The main header for the limes_vecops library.
	@ingroup limes_vecops
 */

#pragma once

// IWYU pragma: begin_exports

#include "./vecops/vecops.h"
#include "./fft/limes_fft.h"
#include "./resampling/limes_resampler.h"
//#include "./filters/limes_filters.h"

// IWYU pragma: end_exports
