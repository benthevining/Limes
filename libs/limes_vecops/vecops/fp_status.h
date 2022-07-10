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

#include <limes_export.h>
#include <limes_namespace.h>

/** @defgroup vec_fpm Floating point mode functions
	Floating point mode functions.
	@ingroup limes_vecops
 */

/** @file
	This file contains floating point mode control functions.
	@ingroup vec_fpm
 */

LIMES_BEGIN_NAMESPACE

namespace vecops
{

#pragma mark Denormals

/* --- Denormals --- */

/** @defgroup vec_denormals Denormalized number support
	Denormalized number support utilities.
	@ingroup vec_fpm
 */

/** @ingroup vec_denormals
	@{
 */

/** Controls whether denormalized numbers are enabled or disabled.
	@see areDenormalsDisabled, ScopedNoDenormals
 */
LIMES_EXPORT void disableDenormalisedNumberSupport (bool shouldDisable = true) noexcept;

/** Returns true if denormalized numbers are currently enabled.
	@see disableDenormalisedNumberSupport
 */
LIMES_EXPORT [[nodiscard]] bool areDenormalsDisabled() noexcept;

/** An RAII class that disables denormalized numbers when it is constructed, and resets the denormalized number state when it is destructed.
	@see disableDenormalisedNumberSupport, areDenormalsDisabled
 */
class LIMES_EXPORT ScopedNoDenormals final
{
public:
	ScopedNoDenormals() noexcept;
	~ScopedNoDenormals() noexcept;

private:
	const intptr_t fpsr;
};

/** @} */

#pragma mark -
#pragma mark Flush-to-zero

/* --- Flush-to-zero --- */

/** @defgroup vec_ftz Flush-to-zero support
	Flush-to-zero support utilities.
	@ingroup vec_fpm
 */

/** @ingroup vec_ftz
	@{
 */

/** Controls whether flush to zero mode is enabled or disabled.
	@see isFlushToZeroEnabled, ScopedFlushToZero
 */
LIMES_EXPORT void enableFlushToZeroMode (bool shouldEnable = true) noexcept;

/** Returns true if flush to zero mode is enabled.
	@see enableFlushToZeroMode
 */
LIMES_EXPORT [[nodiscard]] bool isFlushToZeroEnabled() noexcept;

/** An RAII class that enables flush to zero mode when it is constructed, and resets the flush to zero state when it is destructed.
	@see enableFlushToZeroMode, isFlushToZeroEnabled
 */
class LIMES_EXPORT ScopedFlushToZero final
{
public:
	ScopedFlushToZero() noexcept;
	~ScopedFlushToZero() noexcept;

private:
	const intptr_t fpsr;
};

/** @} */

}  // namespace vecops

LIMES_END_NAMESPACE
