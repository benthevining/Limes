#pragma once

#include <limes_export.h>
#include <limes_namespace.h>
#include "../misc/preprocessor.h"
#include "hour.h"
#include <ctime>
#include <string>

LIMES_BEGIN_NAMESPACE

namespace time
{

class LIMES_EXPORT Time final
{
public:

	constexpr Time() = default;

	explicit constexpr Time (const Hour& h, int min, int sec) noexcept;

	explicit constexpr Time (int hourIn24HourFormat, int min, int sec) noexcept;

	explicit constexpr Time (const std::tm& timeObj) noexcept;

	explicit Time (std::time_t time);

	LIMES_CONSTEXPR_MOVABLE (Time);
	LIMES_CONSTEXPR_COPYABLE (Time);

	[[nodiscard]] constexpr bool isAM() const noexcept;
	[[nodiscard]] constexpr bool isPM() const noexcept;

	[[nodiscard]] constexpr bool isOnTheHour() const noexcept;

	[[nodiscard]] constexpr Hour getHour() const noexcept;
	[[nodiscard]] constexpr int getMinute() const noexcept;
	[[nodiscard]] constexpr int getSecond() const noexcept;

	[[nodiscard]] constexpr bool isBefore (const Time& other) const noexcept;
	[[nodiscard]] constexpr bool isAfter (const Time& other) const noexcept;

	[[nodiscard]] bool isInPast() const noexcept;
	[[nodiscard]] bool isInFuture() const noexcept;

	[[nodiscard]] std::string toString (bool as24HourTime = true) const;

	[[nodiscard]] constexpr bool operator>(const Time& other) const noexcept;
	[[nodiscard]] constexpr bool operator<(const Time& other) const noexcept;

	[[nodiscard]] constexpr bool operator==(const Time& other) const noexcept;
	[[nodiscard]] constexpr bool operator!=(const Time& other) const noexcept;

	[[nodiscard]] static Time getCurrent();

	[[nodiscard]] static consteval Time getCompilationTime() noexcept;

private:
	Hour hour;
	int minute { 0 }, second { 0 };
};

}

LIMES_END_NAMESPACE

#include "impl/time_impl.h"
