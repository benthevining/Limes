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

#include <limes_vecops.h>
#include <limes_platform.h>	 // for LIMES_SSE, LIMES_ARM_NEON, LIMES_INTEL
#include <cstdint>			 // for intptr_t
#include <limes_namespace.h>

#if LIMES_INTEL && LIMES_SSE
#	include <xmmintrin.h>
#endif

#if LIMES_APPLE
#	include <TargetConditionals.h>
#endif

LIMES_BEGIN_NAMESPACE

namespace vecops
{

[[nodiscard]] inline intptr_t getFpStatusRegister() noexcept
{
	intptr_t fpsr = 0;

#if LIMES_INTEL && LIMES_SSE
	return static_cast<intptr_t> (_mm_getcsr());

#elif (defined(__arm64__) || defined(__aarch64__))
#	if TARGET_OS_WATCH
	asm volatile("mrs %w0, fpcr"
				 : "=r"(fpsr));
#	else
	asm volatile("mrs %0, fpcr"
				 : "=r"(fpsr));
#	endif

#elif LIMES_ARM_NEON
	asm volatile("vmrs %0, fpscr"
				 : "=r"(fpsr));

#endif

	return fpsr;
}

inline void setFpStatusRegister (intptr_t fpsr) noexcept
{
#if LIMES_INTEL && LIMES_SSE
	_mm_setcsr (static_cast<uint32_t> (fpsr));

#elif (defined(__arm64__) || defined(__aarch64__))
#	if TARGET_OS_WATCH
	asm volatile("msr fpcr, %w0"
				 :
				 : "ri"(fpsr));
#	else
	asm volatile("msr fpcr, %0"
				 :
				 : "ri"(fpsr));
#	endif

#elif LIMES_ARM_NEON
	asm volatile("vmsr fpscr, %0"
				 :
				 : "ri"(fpsr));

#else
	ignore_unused (fpsr);
#endif
}

[[nodiscard]] constexpr intptr_t getDenormalsMask() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
#	if LIMES_SSE
	return 0x8040;
#	else
	return (1 << 24);
#	endif
#else
	return 0;
#endif
}

[[nodiscard]] constexpr intptr_t getFlushToZeroMask() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
#	if LIMES_SSE
	return _MM_FLUSH_ZERO_MASK;
#	else
	return (1 << 24 /* FZ */);
#	endif
#else
	return 0;
#endif
}

void disableDenormalisedNumberSupport (bool shouldDisable) noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	const auto mask = getDenormalsMask();

	setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldDisable ? mask : 0));
#else
	ignore_unused (shouldDisable);
#endif
}

bool areDenormalsDisabled() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	const auto mask = getDenormalsMask();

	return ((getFpStatusRegister() & mask) == mask);
#else
	return false;
#endif
}

void enableFlushToZeroMode (bool shouldEnable) noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	const auto mask = getFlushToZeroMask();

	setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldEnable ? mask : 0));
#else
	ignore_unused (shouldEnable);
#endif
}

bool isFlushToZeroEnabled() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	const auto mask = getFlushToZeroMask();

	return ((getFpStatusRegister() & mask) == mask);
#else
	return false;
#endif
}

/*---------------------------------------------------------------------------------------------------------------------------*/

ScopedNoDenormals::ScopedNoDenormals() noexcept
	: fpsr (getFpStatusRegister())
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	setFpStatusRegister (fpsr | getDenormalsMask());
#endif
}

ScopedNoDenormals::~ScopedNoDenormals() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	setFpStatusRegister (fpsr);
#endif
}


ScopedFlushToZero::ScopedFlushToZero() noexcept
	: fpsr (getFpStatusRegister())
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	setFpStatusRegister (fpsr | getFlushToZeroMask());
#endif
}

ScopedFlushToZero::~ScopedFlushToZero() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	setFpStatusRegister (fpsr);
#endif
}

}  // namespace vecops

LIMES_END_NAMESPACE
