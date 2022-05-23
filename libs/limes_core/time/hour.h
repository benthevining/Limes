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
#include "../misc/preprocessor.h"
#include "../system/compiler_defs.h"
#include <ctime>

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Hour final
{
public:

	constexpr Hour() = default;

	explicit constexpr Hour (int hoursSinceMidnight) noexcept;

	explicit constexpr Hour (int hourNumber, bool isAM) noexcept;

	explicit constexpr Hour (const std::tm& timeObj) noexcept;

	explicit Hour (std::time_t time);

	LIMES_CONSTEXPR_MOVABLE (Hour);
	LIMES_CONSTEXPR_COPYABLE (Hour);

	[[nodiscard]] constexpr bool isAM() const noexcept;
	[[nodiscard]] constexpr bool isPM() const noexcept;

	[[nodiscard]] constexpr bool isNoon() const noexcept;
	[[nodiscard]] constexpr bool isMidnight() const noexcept;

	[[nodiscard]] constexpr int hoursSinceMidnight() const noexcept;

	[[nodiscard]] constexpr int getIn12HourFormat() const noexcept;

	constexpr Hour& operator++() noexcept;
	constexpr Hour& operator--() noexcept;

	constexpr Hour& operator+= (int hoursToAdd) noexcept;
	constexpr Hour& operator-= (int hoursToSubtract) noexcept;

	[[nodiscard]] constexpr Hour operator+ (int hoursToAdd) const noexcept;
	[[nodiscard]] constexpr Hour operator- (int hoursToSubtract) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Hour& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Hour& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Hour& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Hour& other) const noexcept;

	[[nodiscard]] static Hour getCurrent();

	[[nodiscard]] LIMES_PURE_FUNCTION static consteval Hour getCompilationHour() noexcept;

	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Hour noon() noexcept;
	[[nodiscard]] LIMES_PURE_FUNCTION static constexpr Hour midnight() noexcept;

private:

	int hour { 0 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/hour_impl.h"
