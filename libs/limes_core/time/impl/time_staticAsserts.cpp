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

#include "../year.h"
#include "../month.h"
#include "../weekday.h"
#include "../date.h"
#include "../hour.h"
#include "../../limes_namespace.h"

LIMES_BEGIN_NAMESPACE

namespace time
{

static_assert (Year { 2020 }.isLeap(), "Year::isLeap");
static_assert (! Year { 2021 }.isLeap(), "Year::isLeap");
static_assert (Year { 1976 }.isLeap(), "Year::isLeap");
static_assert (! Year { 1925 }.isLeap(), "Year::isLeap");

static_assert (Month { Month::Name::February }.getNumDays (false) == 28, "Month::getNumDays");
static_assert (Month { Month::Name::February }.getNumDays (true) == 29, "Month::getNumDays");

static_assert (
	(Month { Month::Name::December } ++).getMonthName() == Month::Name::January, "Month::operator++");
static_assert (
	(Month { Month::Name::January } --).getMonthName() == Month::Name::December, "Month::operator--");

static_assert (Month { Month::Name::March }.getMonthNumber() == 3, "Month::getMonthNumber");
static_assert (Month { Month::Name::October }.getMonthNumber() == 10, "Month::getMonthNumber");

static_assert (Month { 4 }.getMonthName() == Month::Name::April, "Month::getMonthName");
static_assert (Month { 6 }.getMonthName() == Month::Name::June, "Month::getMonthName");

static_assert (Weekday<true> { 0 }.toOtherWeekdayType().getString() == Weekday<true> { 0 }.getString());

static_assert (Weekday<true> { 1 }.toOtherWeekdayType().daysSinceStartOfWeek() == 0);
static_assert (Weekday<false> { 0 }.toOtherWeekdayType().daysSinceStartOfWeek() == 1);

static_assert (Weekday<true> { Weekday<true>::Name::Sunday }.daysSinceStartOfWeek() == 0);
static_assert (Weekday<false> { Weekday<false>::Name::Monday }.daysSinceStartOfWeek() == 0);

static_assert (Weekday<true> { 0 }.getWeekdayName() == Weekday<true>::Name::Sunday);
static_assert (Weekday<false> { 0 }.getWeekdayName() == Weekday<false>::Name::Monday);

static_assert (Weekday<true> { 0 }.isWeekend());
static_assert (Weekday<false> { 0 }.isWeekday());

static_assert (Weekday<true> { 1996, 4, 4 }.getWeekdayName() == Weekday<true>::Name::Thursday);
static_assert (Weekday<false> { 1832, 10, 12 }.getWeekdayName() == Weekday<false>::Name::Friday);
static_assert (Weekday<true> { 1692, 2, 2 }.getWeekdayName() == Weekday<true>::Name::Saturday);

static_assert (Hour::midnight().getIn12HourFormat() == 12);
static_assert (Hour { 9 }.getIn12HourFormat() == 9);
static_assert (Hour { 20 }.getIn12HourFormat() == 8);
static_assert (Hour { 7, true }.hoursSinceMidnight() == 7);
static_assert (Hour { 5, false }.hoursSinceMidnight() == 17);

}  // namespace time

LIMES_END_NAMESPACE
