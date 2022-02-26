/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons::music
{

class MicrotonalInterval final
{
public:

	constexpr explicit MicrotonalInterval (double numSemitones)
		: semitones (numSemitones)
	{
	}

	/** Copy constructor. */
	constexpr MicrotonalInterval (const MicrotonalInterval& other)
		: semitones (other.semitones)
	{
	}

	/** Assignment operator. */
	constexpr MicrotonalInterval& operator= (const MicrotonalInterval& other)
	{
		semitones = other.semitones;
		return *this;
	}

	[[nodiscard]] constexpr bool operator== (const MicrotonalInterval& other)
	{
		return semitones == other.semitones;
	}

	[[nodiscard]] constexpr bool operator!= (const MicrotonalInterval& other)
	{
		return semitones != other.semitones;
	}

	[[nodiscard]] constexpr bool operator> (const MicrotonalInterval& other)
	{
		return semitones > other.semitones;
	}

	[[nodiscard]] constexpr bool operator< (const MicrotonalInterval& other)
	{
		return semitones < other.semitones;
	}

	[[nodiscard]] constexpr double getCents() const noexcept
	{
		return semitones * 100.;
	}

	[[nodiscard]] constexpr double getSemitones() const noexcept
	{
		return semitones;
	}

	/** Returns a new pitch object that represents the note this interval's distance above or below the passed pitch object.
		@param other The pitch object to start from.
		@param above When true, the returned pitch will be this interval's distance above the passed pitch. When false, the returned pitch will be this interval's distance below the passed pitch.
	 */
	[[nodiscard]] constexpr Pitch applyToPitch (const Pitch& other, bool above) const noexcept
	{
		if (above)
			return Pitch { other.getMidiPitch() + semitones };

		return Pitch { other.getMidiPitch() - semitones };
	}

private:

	double semitones { 0. };
};


/** Adds an interval to the given pitch and returns a new pitch object at the resulting pitch. */
constexpr Pitch operator+ (const Pitch& pitch, const MicrotonalInterval& interval) noexcept
{
	return Pitch { pitch.getMidiPitch() + interval.getSemitones() };
}

/** Subtracts an interval from the given pitch and returns a new pitch object at the resulting pitch. */
constexpr Pitch operator- (const Pitch& pitch, const MicrotonalInterval& interval) noexcept
{
	return Pitch { pitch.getMidiPitch() - interval.getSemitones() };
}

}  // namespace lemons::music
