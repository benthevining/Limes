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

#include <limes_namespace.h>
#include "../DateTime.h"  // IWYU pragma: associated
#include <string>
#include <sstream>

LIMES_BEGIN_NAMESPACE

namespace time
{

DateTime::DateTime (const Date& d, const Time& t) noexcept
{
	*internal_data.mutable_date() = d.getProtobufObject();
	*internal_data.mutable_time() = t.getProtobufObject();
}

DateTime::DateTime (const std::tm& timeObj) noexcept
{
	const Date d { timeObj };
	const Time t { timeObj };

	*internal_data.mutable_date() = d.getProtobufObject();
	*internal_data.mutable_time() = t.getProtobufObject();
}

template <Clock ClockType>
DateTime::DateTime (const Point<ClockType>& timePoint)
	: DateTime (toTimeObj (timePoint))
{
}

Date DateTime::getDate() const
{
	return Date { static_cast<int> (internal_data.date().year()),
				  static_cast<int> (internal_data.date().month()),
				  static_cast<int> (internal_data.date().day_of_month()) };
}

Time DateTime::getTime() const
{
	return Time {
		static_cast<int> (internal_data.time().hour()),
		static_cast<int> (internal_data.time().minute()),
		static_cast<int> (internal_data.time().second())
	};
}

std::tm DateTime::getStdTime() const noexcept
{
	std::tm res;

	const auto date = getDate();

	res.tm_sec	= internal_data.time().second();
	res.tm_min	= internal_data.time().minute();
	res.tm_hour = internal_data.time().hour();
	res.tm_mday = date.getDayOfMonth();
	res.tm_mon	= date.getMonth().getMonthNumber();
	res.tm_year = date.getYear().getYear() - 1900;
	res.tm_yday = date.getDayOfYear();

	res.tm_wday = date.getWeekday<true>().daysSinceStartOfWeek();

	// set to negative for "no information available"
	res.tm_isdst = -1;

	return res;
}

template <Clock ClockType>
Point<ClockType> DateTime::getTimePoint() const
{
	return fromTimeObj<ClockType> (getTimeT());
}

bool DateTime::isBefore (const DateTime& other) const noexcept
{
	if (getDate().isAfter (other.getDate()))
		return false;

	return getTime().isBefore (other.getTime());
}

bool DateTime::isAfter (const DateTime& other) const noexcept
{
	if (getDate().isBefore (other.getDate()))
		return false;

	return getTime().isAfter (other.getTime());
}

bool DateTime::operator> (const DateTime& other) const noexcept
{
	return isAfter (other);
}

bool DateTime::operator<(const DateTime& other) const noexcept
{
	return isBefore (other);
}

bool DateTime::operator== (const DateTime& other) const noexcept
{
	return internal_data.SerializeAsString() == other.internal_data.SerializeAsString();
}

bool DateTime::operator!= (const DateTime& other) const noexcept
{
	return ! (*this == other);
}

DateTime DateTime::getCompilation() noexcept
{
	return DateTime { Date::getCompilationDate(), Time::getCompilationTime() };
}

DateTime::DateTime (std::time_t t)
	: DateTime (*std::localtime (&t))
{
}

std::time_t DateTime::getTimeT() const
{
	auto tm = getStdTime();

	return std::mktime (&tm);
}

bool DateTime::isInPast() const noexcept
{
	return isBefore (getCurrent());
}

bool DateTime::isInFuture() const noexcept
{
	return isAfter (getCurrent());
}

std::string DateTime::toString (bool as24HourTime, bool shortMonthName) const
{
	std::stringstream stream;

	stream << getTime().toString (as24HourTime) << ' ' << getDate().toString (shortMonthName);

	return stream.str();
}

}  // namespace time

LIMES_END_NAMESPACE
