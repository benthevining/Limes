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

#include "Filter.h"
#include <limes_namespace.h>
#include <limes_core.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::filters
{

template <Sample SampleType>
static LIMES_FORCE_INLINE void snapToZero (SampleType& sample) noexcept
{
	if (! (sample < SampleType (-1.0e-8) || sample > SampleType (1.0e-8)))
		sample = SampleType (0);
}

template <Sample SampleType>
void Filter<SampleType>::reset (SampleType resetToValue) noexcept
{
	order = coefs.getFilterOrder();

	state.reserve (static_cast<typename std::vector<SampleType>::size_type>(order));

	alg::fill (state, resetToValue);
}

template <Sample SampleType>
void Filter<SampleType>::prepare() noexcept
{
	reset();
}

template <Sample SampleType>
void Filter<SampleType>::processOrder1 (SampleType* buffer, int numSamples) noexcept
{
	const auto* const coeffs = coefs.getRawCoefficients();

	const auto b0 = coeffs[0];
	const auto b1 = coeffs[1];
	const auto a1 = coeffs[2];

	auto lv1 = state[0];

	for (auto i = 0; i < numSamples; ++i)
	{
		const auto input  = buffer[i];
		const auto output = input * b0 + lv1;

		buffer[i] = output;

		lv1 = (input * b1) - (output * a1);
	}

	snapToZero (lv1);

	state[0] = lv1;
}

template <Sample SampleType>
void Filter<SampleType>::processOrder2 (SampleType* buffer, int numSamples) noexcept
{
	const auto* const coeffs = coefs.getRawCoefficients();

	const auto b0 = coeffs[0];
	const auto b1 = coeffs[1];
	const auto b2 = coeffs[2];
	const auto a1 = coeffs[3];
	const auto a2 = coeffs[4];

	auto lv1 = state[0];
	auto lv2 = state[1];

	for (auto i = 0; i < numSamples; ++i)
	{
		const auto input  = buffer[i];
		const auto output = (input * b0) + lv1;

		buffer[i] = output;

		lv1 = (input * b1) - (output * a1) + lv2;
		lv2 = (input * b2) - (output * a2);
	}

	snapToZero (lv1);
	snapToZero (lv2);

	state[0] = lv1;
	state[1] = lv2;
}

template <Sample SampleType>
void Filter<SampleType>::processOrder3 (SampleType* buffer, int numSamples) noexcept
{
	const auto* const coeffs = coefs.getRawCoefficients();

	const auto b0 = coeffs[0];
	const auto b1 = coeffs[1];
	const auto b2 = coeffs[2];
	const auto b3 = coeffs[3];
	const auto a1 = coeffs[4];
	const auto a2 = coeffs[5];
	const auto a3 = coeffs[6];

	auto lv1 = state[0];
	auto lv2 = state[1];
	auto lv3 = state[2];

	for (auto i = 0; i < numSamples; ++i)
	{
		const auto input  = buffer[i];
		const auto output = (input * b0) + lv1;

		buffer[i] = output;

		lv1 = (input * b1) - (output * a1) + lv2;
		lv2 = (input * b2) - (output * a2) + lv3;
		lv3 = (input * b3) - (output * a3);
	}

	snapToZero (lv1);
	snapToZero (lv2);
	snapToZero (lv3);

	state[0] = lv1;
	state[1] = lv2;
	state[2] = lv3;
}

template <Sample SampleType>
void Filter<SampleType>::processDefault (SampleType* buffer, int numSamples) noexcept
{
	const auto* const coeffs = coefs.getRawCoefficients();

	for (auto i = 0; i < numSamples; ++i)
	{
		const auto input  = buffer[i];
		const auto output = (input * coeffs[0]) + state[0];

		buffer[i] = output;

		for (auto j = 0UL; j < order - 1; ++j)
			state[j] = (input * coeffs[j + 1])
					 - (output * coeffs[order + j + 1]) + state[j + 1];

		state[order - 1] = (input * coeffs[order]) - (output * coeffs[static_cast<ptrdiff_t> (order * 2)]);
	}

	for (auto& e : state)
		snapToZero (e);
}

template <Sample SampleType>
void Filter<SampleType>::process (SampleType* buffer, int numSamples) noexcept
{
	if (order != coefs.getFilterOrder())
		reset();

	switch (order)
	{
		case 1 :
		{
			processOrder1 (buffer, numSamples);
			break;
		}

		case 2 :
		{
			processOrder2 (buffer, numSamples);
			break;
		}

		case 3 :
		{
			processOrder3 (buffer, numSamples);
			break;
		}

		default :
		{
			processDefault (buffer, numSamples);
			break;
		}
	}
}

template <Sample SampleType>
void Filter<SampleType>::process (Buffer& buffer) noexcept
{
	process (buffer.getWritePointer (0), buffer.getNumSamples());
}

template <Sample SampleType>
SampleType Filter<SampleType>::processSample (SampleType inputSample) noexcept
{
	process (&inputSample, 1);

	return inputSample;
}

template class Filter<float>;
template class Filter<double>;

}  // namespace dsp::filters

LIMES_END_NAMESPACE
