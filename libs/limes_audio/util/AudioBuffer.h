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
#include "Misc.h"
#include <limes_vecops.h>
#include <limes_core.h>
#include <array>

/** @file
	This file defines the AudioBuffer class.
	@ingroup limes_audio
 */

LIMES_BEGIN_NAMESPACE

namespace dsp
{

/** A multi-channel audio buffer.
	All operations use the functions from limes_vecops where possible.
	@ingroup limes_audio
 */
template <Sample SampleType, std::size_t NumChannels>
class LIMES_EXPORT AudioBuffer final
{
public:

	// alias constructor
	// make copy of
	// clear range of samples, or just a single channel
	// reverse
	// convert sample types
	// interleave/deinterleave
	// swapWith?

	using Type = SampleType;

	using ChannelPointer = memory::array_pointer<SampleType, false>;

	/** @name Constructors */
	///@{
	AudioBuffer() = default;

	explicit AudioBuffer (std::size_t numSamples, bool clear = true)
	{
		for (std::size_t i = 0; i < NumChannels; ++i)
		{
			channels[i].reallocate (numSamples);

			if (clear)
				vecops::clear<SampleType> (channels[i], numSamples);
		}
	}
	///@}

	LIMES_DEFAULT_COPYABLE (AudioBuffer)
	LIMES_DEFAULT_MOVABLE (AudioBuffer)

	static constexpr int getNumChannels() noexcept
	{
		return static_cast<int> (NumChannels);
	}

	int getNumSamples() const noexcept
	{
		if constexpr (NumChannels == 0)
			return 0;

		return channels[0].getSize();
	}

	const SampleType* getReadPointer (std::size_t channel, std::size_t sampleIndex = 0) const noexcept
	{
		LIMES_ASSERT (channel < NumChannels);
		LIMES_ASSERT (static_cast<int>(sampleIndex) < getNumSamples());

		return channels[channel] + sampleIndex;
	}

	SampleType* getWritePointer (std::size_t channel, std::size_t sampleIndex = 0) noexcept
	{
		LIMES_ASSERT (channel < NumChannels);
		LIMES_ASSERT (static_cast<int>(sampleIndex) < getNumSamples());

		return channels[channel] + sampleIndex;
	}

	ChannelPointer& getChannelPointer (std::size_t channel) noexcept
	{
		LIMES_ASSERT (channel < NumChannels);

		return channels[channel];
	}

	const ChannelPointer& getChannelPointer (std::size_t channel) const noexcept
	{
		LIMES_ASSERT (channel < NumChannels);

		return channels[channel];
	}

	void resize (std::size_t newNumSamples, bool clearNewSamples = true)
	{
		const auto oldNumSamples = getNumSamples();
		const auto numNewSamples = static_cast<int> (newNumSamples) - oldNumSamples;

		for (std::size_t i = 0; i < NumChannels; ++i)
		{
			channels[i].reallocate (newNumSamples);

			if (clearNewSamples && numNewSamples > 0)
				vecops::clear<SampleType> (channels[i] + oldNumSamples, numNewSamples);
		}
	}

	void resize (int newNumSamples, bool clearNewSamples = true)
	{
		LIMES_ASSERT(newNumSamples >= 0);
		resize(static_cast<std::size_t>(newNumSamples), clearNewSamples);
	}

	void deallocate()
	{
		for (std::size_t i = 0; i < NumChannels; ++i)
			channels[i].free();
	}

	void clear() noexcept
	{
		const auto numSamples = getNumSamples();

		for (std::size_t i = 0; i < NumChannels; ++i)
			vecops::clear<SampleType> (channels[i], numSamples);
	}

	SampleType getSample (std::size_t channel, std::size_t sampleIndex) const noexcept
	{
		LIMES_ASSERT (channel < NumChannels);
		LIMES_ASSERT (static_cast<int>(sampleIndex) < getNumSamples());

		return channels[channel][sampleIndex];
	}

	void setSample (std::size_t channel, std::size_t sampleIndex, SampleType sampleValue) noexcept
	{
		LIMES_ASSERT (channel < NumChannels);
		LIMES_ASSERT (sampleIndex < getNumSamples());

		channels[channel][sampleIndex] = sampleValue;
	}

	void applyGain (SampleType gain) noexcept
	{
		const auto numSamples = getNumSamples();

		for (std::size_t i = 0; i < NumChannels; ++i)
			vecops::multiply<SampleType> (channels[i], numSamples, gain);
	}

	void applyGainRamp (SampleType startGain, SampleType endGain) noexcept
	{
		const auto numSamples = getNumSamples();

		for (std::size_t i = 0; i < NumChannels; ++i)
			vecops::applyRamp<SampleType> (channels[i], numSamples, startGain, endGain);
	}

	void applyWindowFunction (vecops::window::Type windowType) noexcept
	{
		const auto numSamples = getNumSamples();

		for (std::size_t i = 0; i < NumChannels; ++i)
			vecops::window::apply<SampleType> (channels[i], numSamples, windowType);
	}

	void copyFrom (const SampleType* data, std::size_t dataSize, std::size_t destChannel, std::size_t destStartSample = 0) noexcept
	{
		LIMES_ASSERT (destChannel < NumChannels);
		LIMES_ASSERT (static_cast<int>(destStartSample + dataSize) <= getNumSamples());

		vecops::copy<SampleType> (channels[destChannel] + destStartSample, data, dataSize);
	}

	void addFrom (const SampleType* data, std::size_t dataSize, std::size_t destChannel, std::size_t destStartSample = 0) noexcept
	{
		LIMES_ASSERT (destChannel < NumChannels);
		LIMES_ASSERT (static_cast<int>(destStartSample + dataSize) <= getNumSamples());

		vecops::add<SampleType> (channels[destChannel] + destStartSample, dataSize, data);
	}

	void copyTo (SampleType* dest, std::size_t channelToCopyFrom, std::size_t maxSamplesToCopy) noexcept
	{
		LIMES_ASSERT (channelToCopyFrom < NumChannels);

		vecops::copy<SampleType> (dest, channels[channelToCopyFrom], std::min (static_cast<int> (maxSamplesToCopy), getNumSamples()));
	}

	SampleType getRMS (std::size_t channel) const noexcept
	{
		LIMES_ASSERT (channel < NumChannels);

		return vecops::rms<SampleType> (channels[channel], getNumSamples());
	}

	void clip (SampleType minClip = SampleType (-1), SampleType maxClip = SampleType (1)) noexcept
	{
		const auto numSamples = getNumSamples();

		for (std::size_t i = 0; i < NumChannels; ++i)
			vecops::clip<SampleType> (channels[i], numSamples, minClip, maxClip);
	}

	vecops::VectorExtremaInfo<SampleType> findMinMax (std::size_t channel) noexcept
	{
		LIMES_ASSERT (channel < NumChannels);

		return vecops::getVectorExtremaInfo<SampleType> (channels[channel], getNumSamples());
	}

	vecops::VectorExtremaInfo<SampleType> findAbsMinMax (std::size_t channel) noexcept
	{
		LIMES_ASSERT (channel < NumChannels);

		return vecops::getAbsVectorExtremaInfo<SampleType> (channels[channel], getNumSamples());
	}

	template <std::size_t NewNumChannels>
	[[nodiscard]] AudioBuffer<SampleType, NewNumChannels> getAlias (std::size_t startChannel,
																	std::size_t startSample,
																	std::size_t numSamples) noexcept
	{
		static_assert (NewNumChannels <= NumChannels);

		if constexpr (NewNumChannels == 0)
			return {};

		LIMES_ASSERT (startSample + numSamples <= getNumSamples());

		AudioBuffer<SampleType, NewNumChannels> newBuffer;

		for (std::size_t i = 0, j = startChannel; i < NumChannels; ++i, ++j)
			newBuffer.channels[i].referenceOtherMemory (channels[j] + startSample, numSamples);

		return newBuffer;
	}

private:

	std::array<ChannelPointer, NumChannels> channels;
};

}  // namespace dsp

LIMES_END_NAMESPACE
