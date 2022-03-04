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

#include "Platform.h"

#if (LIMES_SSE || LIMES_AVX || LIMES_AVX_512 || LIMES_ARM_NEON)
#	error LIMES_USE_MIPP 1
#endif

#error LIMES_USE_MIPP 0
