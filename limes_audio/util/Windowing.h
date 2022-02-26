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

#include <limes_vecops.h>
#include <functional>

namespace lemons::dsp::windowing
{

template <typename ValueType>
static const auto rectangular = [] (int, int)
{ return ValueType (1); };

template <typename ValueType>
static const auto triangular = [] (int size, int i)
{
	const auto halfSlots = ValueType (0.5) * static_cast<ValueType> (size - 1);

	return static_cast<ValueType> (1.0) - std::abs ((static_cast<ValueType> (i) - halfSlots) / halfSlots);
};

namespace detail
{

template <typename ValueType>
constexpr ValueType ncos (int order, int i, int size) noexcept
{
	return std::cos (static_cast<ValueType> (order * i)
					 * constants::pi<ValueType> / static_cast<ValueType> (size - 1));
}

}  // namespace detail

template <typename ValueType>
static const auto hann = [] (int size, int i)
{
	return static_cast<ValueType> (0.5 - 0.5 * detail::ncos<ValueType> (2, i, size));
};

template <typename ValueType>
static const auto hamming = [] (int size, int i)
{
	return static_cast<ValueType> (0.54 - 0.46 * detail::ncos<ValueType> (2, i, size));
};

template <typename ValueType>
static const auto blackman = [] (int size, int i)
{
	constexpr auto alpha = ValueType (0.16);

	const auto cos2 = detail::ncos<ValueType> (2, i, size);
	const auto cos4 = detail::ncos<ValueType> (4, i, size);

	return static_cast<ValueType> (0.5 * (1 - alpha) - 0.5 * cos2 + 0.5 * alpha * cos4);
};

template <typename ValueType>
static const auto blackmanHarris = [] (int size, int i)
{
	const auto cos2 = detail::ncos<ValueType> (2, i, size);
	const auto cos4 = detail::ncos<ValueType> (4, i, size);
	const auto cos6 = detail::ncos<ValueType> (6, i, size);

	return static_cast<ValueType> (0.35875 - 0.48829 * cos2 + 0.14128 * cos4 - 0.01168 * cos6);
};

template <typename ValueType>
static const auto flatTop = [] (int size, int i)
{
	const auto cos2 = detail::ncos<ValueType> (2, i, size);
	const auto cos4 = detail::ncos<ValueType> (4, i, size);
	const auto cos6 = detail::ncos<ValueType> (6, i, size);
	const auto cos8 = detail::ncos<ValueType> (8, i, size);

	return static_cast<ValueType> (1.0 - 1.93 * cos2 + 1.29 * cos4 - 0.388 * cos6 + 0.028 * cos8);
};

template <typename ValueType>
using WindowingFunction = std::function<ValueType (int, int)>;

template <typename ValueType>
void makeWindow (ValueType* const windowOut, int windowSize, const WindowingFunction<ValueType>& func)
{
	for (auto i = 0; i < windowSize; ++i)
		windowOut[i] = func (windowSize, i);
}

template <typename ValueType>
void applyWindow (ValueType* const inputSamples, int numSamples, const WindowingFunction<ValueType>& func)
{
	for (auto i = 0; i < numSamples; ++i)
		inputSamples[i] *= func (numSamples, i);
}

template <typename ValueType>
void applyWindowAndCopy (const ValueType* const inputSamples, ValueType* const outputSamples,
						 int numSamples, const WindowingFunction<ValueType>& func)
{
	for (auto i = 0; i < numSamples; ++i)
		outputSamples[i] = inputSamples[i] * func (numSamples, i);
}

}  // namespace lemons::dsp::windowing
