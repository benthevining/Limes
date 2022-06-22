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

#include <limes_namespace.h>
#include <limes_export.h>
#include "mathFunctions.h"	// for concept Scalar
#include <cmath>
#include <limits>

/** @file
	This file defines the StatsComputer class.
	@ingroup core_math
 */

LIMES_BEGIN_NAMESPACE

namespace math
{

/** This class computes some running statistics based on a stream of input data.
	Internally, Welford's algorithm @cite welford_1962 is used.
	@ingroup core_math

	@todo static assertion tests
 */
template <Scalar T>
class LIMES_EXPORT StatsComputer final
{
public:

	/** You can access the type of input data using this typedef. */
	using DataType = T;

	/** Inputs a new value to the accumulator. */
	constexpr void newValue (T x) noexcept
	{
		++numData;

		const auto delta = static_cast<double> (x) - mu;

		mu += delta / static_cast<double> (numData);
		m += delta * delta;

		m_sum += x;

		m_max = std::max (x, m_max);
		m_min = std::min (x, m_min);
	}

	/** Returns the sum of the stream of data. */
	constexpr T sum() const noexcept { return m_sum; }

	/** Returns the maximum of the stream of data. */
	constexpr T max() const noexcept { return m_max; }

	/** Returns the minimum of the stream of data. */
	constexpr T min() const noexcept { return m_min; }

	/** Returns the mean of the stream of data. */
	constexpr double mean() const noexcept
	{
		return mu;
	}

	/** Returns the variance of the stream of data. */
	constexpr double variance() const noexcept
	{
		return m / static_cast<double> (numData);
	}

	/** Returns the standard deviation of the stream of data. */
	constexpr T standardDeviation() const noexcept
	{
		return std::sqrt (variance());
	}

	/** Returns the number of data points that have been fed to \c newValue() so far. */
	constexpr int datasetSize() const noexcept { return static_cast<int> (numData); }

	/** Resets the state of the accumulator. */
	constexpr void reset() noexcept
	{
		numData = 0;
		m_sum	= 0;
		mu		= 0.;
		var		= 0.;
		m		= 0.;

		m_max = std::numeric_limits<T>::min();
		m_min = std::numeric_limits<T>::max();
	}

private:

	std::size_t numData { 0 };

	double mu { 0. }, var { 0. }, m { 0. };

	T m_sum { 0 };
	T m_max { std::numeric_limits<T>::min() };
	T m_min { std::numeric_limits<T>::max() };
};

}  // namespace math

LIMES_END_NAMESPACE
