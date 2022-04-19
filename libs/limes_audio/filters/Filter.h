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
#include <limes_data_structures.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::filters
{

/** A basic IIR filter with coefficients that can be reassigned during realtime playback.
 */
template <Sample SampleType>
class LIMES_EXPORT Filter final
{
public:

	using SampleVector = scalar_vector<SampleType>;

	/** Resets the internal state of the filter. */
	void reset (SampleType resetToValue = SampleType (0));

	/** Prepares the filter to process audio. */
	void prepare() noexcept;

	/** Applies the filter to a stream of audio samples. */
	void process (SampleType* buffer, int numSamples);

	void process (SampleVector& buffer);

	/** The filter's coefficients.
		You can call various methods of this object to create different kinds of filters.
	*/
	Coeffecients<SampleType> coefs;

private:

	void processOrder1 (SampleType* buffer, int numSamples);
	void processOrder2 (SampleType* buffer, int numSamples);
	void processOrder3 (SampleType* buffer, int numSamples);
	void processDefault (SampleType* buffer, int numSamples);

	SampleVector state;
	int			 order = 0;
};

}  // namespace dsp::filters

LIMES_END_NAMESPACE
