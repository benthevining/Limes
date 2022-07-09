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

#include "../limes_vecops.h"  // IWYU pragma: associated
#include <limes_platform.h>	  // for LIMES_SSE, LIMES_ARM_NEON, LIMES_INTEL
#include <cstdint>			  // for intptr_t
#include <limes_namespace.h>
#include <limes_core.h>

#if LIMES_INTEL && LIMES_SSE
#	include <xmmintrin.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops
{

[[nodiscard]] static LIMES_FORCE_INLINE intptr_t getFpStatusRegister() noexcept
{
	intptr_t fpsr = 0;

#if LIMES_INTEL && LIMES_SSE
	return static_cast<intptr_t> (_mm_getcsr());

#elif (defined(__arm64__) || defined(__aarch64__))
	asm volatile("mrs %0, fpcr"
				 : "=r"(fpsr));

#elif LIMES_ARM_NEON
	asm volatile("vmrs %0, fpscr"
				 : "=r"(fpsr));

#endif

	return fpsr;
}

LIMES_FORCE_INLINE static void setFpStatusRegister (intptr_t fpsr) noexcept
{
#if LIMES_INTEL && LIMES_SSE
	_mm_setcsr (static_cast<uint32_t> (fpsr));

#elif (defined(__arm64__) || defined(__aarch64__))
	asm volatile("msr fpcr, %0"
				 :
				 : "ri"(fpsr));

#elif LIMES_ARM_NEON
	asm volatile("vmsr fpscr, %0"
				 :
				 : "ri"(fpsr));

#else
	misc::ignore_unused (fpsr);
#endif
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#undef LIMES__HAS_FP_STATUS_SUPPORT__

#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
#	define LIMES__HAS_FP_STATUS_SUPPORT__
#endif

#pragma mark -
#pragma mark Denormals

[[nodiscard]] static consteval intptr_t getDenormalsMask() noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__

#	if LIMES_SSE
	return 0x8040;
#	else
	return (1 << 24);
#	endif

#else /* LIMES__HAS_FP_STATUS_SUPPORT__ */
	return 0;
#endif
}

void disableDenormalisedNumberSupport (bool shouldDisable) noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__

	const auto mask = getDenormalsMask();

	setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldDisable ? mask : 0));
#else
	misc::ignore_unused (shouldDisable);
#endif
}

bool areDenormalsDisabled() noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__

	const auto mask = getDenormalsMask();

	return ((getFpStatusRegister() & mask) == mask);
#else
	return false;
#endif
}

ScopedNoDenormals::ScopedNoDenormals() noexcept
	: fpsr (getFpStatusRegister())
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__
	setFpStatusRegister (fpsr | getDenormalsMask());
#endif
}

ScopedNoDenormals::~ScopedNoDenormals() noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__
	setFpStatusRegister (fpsr);
#endif
}

/*---------------------------------------------------------------------------------------------------------------------------*/

#pragma mark -
#pragma mark Flush-to-zero

[[nodiscard]] static constexpr intptr_t getFlushToZeroMask() noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__

#	if LIMES_SSE
	return _MM_FLUSH_ZERO_MASK;
#	else
	return (1 << 24 /* FZ */);
#	endif

#else /* LIMES__HAS_FP_STATUS_SUPPORT__ */
	return 0;
#endif
}

void enableFlushToZeroMode (bool shouldEnable) noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__

	const auto mask = getFlushToZeroMask();

	setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldEnable ? mask : 0));

#else
	misc::ignore_unused (shouldEnable);
#endif
}

bool isFlushToZeroEnabled() noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__
	const auto mask = getFlushToZeroMask();

	return ((getFpStatusRegister() & mask) == mask);

#else
	return false;
#endif
}

ScopedFlushToZero::ScopedFlushToZero() noexcept
	: fpsr (getFpStatusRegister())
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__
	setFpStatusRegister (fpsr | getFlushToZeroMask());
#endif
}

ScopedFlushToZero::~ScopedFlushToZero() noexcept
{
#ifdef LIMES__HAS_FP_STATUS_SUPPORT__
	setFpStatusRegister (fpsr);
#endif
}


#undef LIMES__HAS_FP_STATUS_SUPPORT__

}  // namespace vecops

LIMES_END_NAMESPACE
