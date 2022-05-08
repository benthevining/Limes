#pragma once

#include <limes_export.h>
#include <limes_namespace.h>
#include "time.h"
#include "date.h"
#include <ctime>
#include <string>

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT DateTime final
{
public:

	constexpr DateTime() = default;

	explicit constexpr DateTime (const Date& d, const Time& t) noexcept;

	explicit constexpr DateTime (const std::tm& timeObj) noexcept;

	explicit DateTime (std::time_t t);

	LIMES_CONSTEXPR_MOVABLE (DateTime);
	LIMES_CONSTEXPR_COPYABLE (DateTime);

	[[nodiscard]] constexpr std::tm getStdTime() const noexcept;

	[[nodiscard]] std::time_t getTimeT() const;

	[[nodiscard]] constexpr bool isBefore (const DateTime& other) const noexcept;
	[[nodiscard]] constexpr bool isAfter (const DateTime& other) const noexcept;

	[[nodiscard]] bool isInPast() const noexcept;
	[[nodiscard]] bool isInFuture() const noexcept;

	[[nodiscard]] std::string toString (bool as24HourTime = true, bool shortMonthName = true) const;

	[[nodiscard]] constexpr bool operator> (const DateTime& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const DateTime& other) const noexcept;

	[[nodiscard]] constexpr bool operator== (const DateTime& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const DateTime& other) const noexcept;

	[[nodiscard]] static DateTime getCurrent();

	[[nodiscard]] static consteval DateTime getCompilation() noexcept;

private:
	Time time;
	Date date;
};

/*-----------------------------------------------------------------------------------------------------------------------*/

constexpr DateTime::DateTime (const Date& d, const Time& t) noexcept
: time(t), date(d)
{

}

constexpr DateTime::DateTime (const std::tm& timeObj) noexcept
: time(timeObj), date(timeObj)
{

}

constexpr std::tm DateTime::getStdTime() const noexcept
{
	std::tm res;

	res.tm_sec = time.getSecond();
	res.tm_min = time.getMinute();
	res.tm_hour = time.getHour().hoursSinceMidnight();
	res.tm_mday = date.getDayOfMonth();
	res.tm_mon = date.getMonth().getMonthNumber();
	res.tm_year = date.getYear().getYear() - 1900;
	res.tm_wday = date.getWeekday<true>().daysSinceStartOfWeek();
	res.tm_yday = date.getDayOfYear();

	// set to negative for "no information available"
	res.tm_isdst = -1;

	return res;
}

constexpr bool DateTime::isBefore (const DateTime& other) const noexcept
{
	if (date.isAfter (other.date))
		return false;

	return time.isBefore (other.time);
}

constexpr bool DateTime::isAfter (const DateTime& other) const noexcept
{
	if (date.isBefore (other.date))
		return false;

	return time.isAfter (other.time);
}

constexpr bool DateTime::operator> (const DateTime& other) const noexcept
{
	return isAfter (other);
}

constexpr bool DateTime::operator< (const DateTime& other) const noexcept
{
	return isBefore (other);
}

constexpr bool DateTime::operator== (const DateTime& other) const noexcept
{
	return date == other.date && time == other.time;
}

constexpr bool DateTime::operator!= (const DateTime& other) const noexcept
{
	return ! (*this == other);
}

consteval DateTime DateTime::getCompilation() noexcept
{
	return DateTime{ Date::getCompilationDate(), Time::getCompilationTime() };
}

}

LIMES_END_NAMESPACE
