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
#include "weekday.h"
#include "month.h"
#include "year.h"
#include <ctime>
#include <string>
#include "time_utils.h"
#include "../system/compiler_defs.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Date final
{
public:

	constexpr Date() = default;

	explicit constexpr Date (const Year& y, const Month& m, int d) noexcept;

	explicit constexpr Date (int y, int m, int d) noexcept;

	explicit constexpr Date (const std::tm& timeObj) noexcept;

	explicit Date (std::time_t time);

	template <Clock ClockType>
	explicit Date (const Point<ClockType>& timePoint);

	LIMES_CONSTEXPR_MOVABLE (Date);
	LIMES_CONSTEXPR_COPYABLE (Date);

	[[nodiscard]] constexpr Year  getYear() const noexcept;
	[[nodiscard]] constexpr Month getMonth() const noexcept;
	[[nodiscard]] constexpr int	  getDayOfMonth() const noexcept;
	[[nodiscard]] constexpr int	  getDayOfYear() const noexcept;

	template <bool StartWeekOnSunday = false>
	[[nodiscard]] constexpr Weekday<StartWeekOnSunday> getWeekday() const noexcept;

	[[nodiscard]] constexpr int getWeekNumber() const noexcept;

	[[nodiscard]] bool isInPast() const noexcept;
	[[nodiscard]] bool isInFuture() const noexcept;

	[[nodiscard]] constexpr bool isBefore (const Date& other) const noexcept;
	[[nodiscard]] constexpr bool isAfter (const Date& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Date& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Date& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const Date& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Date& other) const noexcept;

	[[nodiscard]] std::string toString (bool shortMonthName = true) const;

	[[nodiscard]] static Date getCurrent();

	[[nodiscard]] LIMES_PURE_FUNCTION static consteval Date getCompilationDate() noexcept;

private:

	Year  year;
	Month month;
	int	  dayOfMonth { 1 };
};

}  // namespace time

LIMES_END_NAMESPACE

#include "impl/date_impl.h"
