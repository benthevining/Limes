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
#include <limes_platform.h>

namespace limes::vecops
{

[[nodiscard]] inline intptr_t getFpStatusRegister() noexcept
{
	intptr_t fpsr = 0;

#if LIMES_INTEL && LIMES_SSE
	return static_cast<intptr_t> (_mm_getcsr());

#elif defined(__arm64__) || defined(__aarch64__)
	asm volatile("mrs %0, fpcr"
				 : "=r"(fpsr));

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

#elif defined(__arm64__) || defined(__aarch64__)
	asm volatile("msr fpcr, %0"
				 :
				 : "ri"(fpsr));

#elif LIMES_ARM_NEON
	asm volatile("vmsr fpscr, %0"
				 :
				 : "ri"(fpsr));

#else
	(void) fpsr;  // ignore unused variable
#endif
}

void disableDenormalisedNumberSupport (bool shouldDisable) noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
#	if LIMES_SSE
	constexpr intptr_t mask = 0x8040;
#	else
	constexpr intptr_t mask = (1 << 24);
#	endif

	setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldDisable ? mask : 0));
#else
	(void) shouldDisable;  // ignore unused variable
#endif
}

bool areDenormalsDisabled() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
#	if LIMES_SSE
	constexpr intptr_t mask = 0x8040;
#	else
	constexpr intptr_t mask = (1 << 24);
#	endif

	return ((getFpStatusRegister() & mask) == mask);
#else
	return false;
#endif
}

void enableFlushToZeroMode (bool shouldEnable) noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
#	if LIMES_SSE
	constexpr intptr_t mask = _MM_FLUSH_ZERO_MASK;
#	else
	constexpr intptr_t mask = (1 << 24 /* FZ */);
#	endif

	setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldEnable ? mask : 0));
#else
	(void) shouldEnable;  // ignore unused variable
#endif
}

/*---------------------------------------------------------------------------------------------------------------------------*/

ScopedNoDenormals::ScopedNoDenormals() noexcept
	: fpsr (getFpStatusRegister())
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
#	if LIMES_SSE
	constexpr intptr_t mask = 0x8040;
#	else
	constexpr intptr_t mask = (1 << 24 /* FZ */);
#	endif

	setFpStatusRegister (fpsr | mask);
#endif
}

ScopedNoDenormals::~ScopedNoDenormals() noexcept
{
#if LIMES_SSE || (LIMES_ARM_NEON || defined(__arm64__) || defined(__aarch64__))
	setFpStatusRegister (fpsr);
#endif
}

}  // namespace limes::vecops
