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
#include "Coeffecients.h"
#include "../util/Misc.h"
#include "../util/AudioBuffer.h"
#include <limes_namespace.h>

/** @file
	This file defines the Filter class.
	@ingroup dsp_filters
 */

LIMES_BEGIN_NAMESPACE

namespace dsp::filters
{

/** A basic IIR filter with coefficients that can be reassigned during realtime playback.
	@ingroup dsp_filters

	@todo add a higher-level API for switching filter type & freq during playback
 */
template <Sample SampleType>
class LIMES_EXPORT Filter final
{
public:

	using Buffer = AudioBuffer<SampleType, 1>;

	/** Resets the internal state of the filter. */
	void reset (SampleType resetToValue = SampleType (0)) noexcept;

	/** Prepares the filter to process audio. */
	void prepare() noexcept;

	/** @name Processing streams of samples */
	///@{
	/** Applies the filter to a stream of audio samples in place. */
	void process (SampleType* buffer, int numSamples) noexcept;
	void process (Buffer& buffer) noexcept;
	///@}

	/** Processes a single sample of input. */
	SampleType processSample (SampleType inputSample) noexcept;

	/** The filter's coefficients.
		You can call various methods of this object to create different kinds of filters.
	*/
	Coeffecients<SampleType> coefs;

private:

	void processOrder1 (SampleType* buffer, int numSamples) noexcept;
	void processOrder2 (SampleType* buffer, int numSamples) noexcept;
	void processOrder3 (SampleType* buffer, int numSamples) noexcept;
	void processDefault (SampleType* buffer, int numSamples) noexcept;

	std::vector<SampleType> state;

	std::size_t order = 0;
};

}  // namespace dsp::filters

LIMES_END_NAMESPACE
