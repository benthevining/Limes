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
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace music
{

struct LIMES_EXPORT NoteDuration final
{
public:

	// TO DO: triplets

	// special: double whole note

	constexpr explicit NoteDuration (int kind);

	LIMES_CONSTEXPR_MOVABLE (NoteDuration);
	LIMES_CONSTEXPR_COPYABLE (NoteDuration);

	[[nodiscard]] constexpr bool operator== (const NoteDuration& other) const noexcept;

	[[nodiscard]] constexpr bool operator!= (const NoteDuration& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const NoteDuration& other) const noexcept;

	[[nodiscard]] constexpr bool operator<(const NoteDuration& other) const noexcept;

	constexpr NoteDuration& operator++() noexcept;

	constexpr NoteDuration& operator--() noexcept;

	[[nodiscard]] constexpr bool hasStem() const noexcept;

	[[nodiscard]] constexpr bool noteHeadIsFilled() const noexcept;

	[[nodiscard]] constexpr int getDuration() const noexcept;

	[[nodiscard]] constexpr double getRatioToOtherDuration (const NoteDuration& other) const noexcept;

private:

	int duration { 4 };
};

}  // namespace music

LIMES_END_NAMESPACE

#include "NoteDuration_impl.h"
