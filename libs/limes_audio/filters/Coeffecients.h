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

#include "../util/Misc.h"
#include <limes_data_structures.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::filters
{

/** A filter coefficients class, with lots of methods for creating various filter topologies.
	This class is designed so that coefficients can be recreated and reassigned during realtime playback; no allocations should occur.
 */
template <Sample Sampletype>
class LIMES_EXPORT Coeffecients final
{
public:

	/** Creates the coefficients for a first order low pass filter. */
	void makeFirstOrderLowPass (double sampleRate, Sampletype frequency);

	/** Creates the coefficients for a first order high pass filter. */
	void makeFirstOrderHighPass (double sampleRate, Sampletype frequency);

	/** Creates the coefficients for a first order all-pass filter. */
	void makeFirstOrderAllPass (double sampleRate, Sampletype frequency);

	/** Creates the coefficients for a low pass filter, with variable Q. */
	void makeLowPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>);

	/** Creates the coefficients for a high pass filter, with variable Q. */
	void makeHighPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>);

	/** Creates the coefficients for a band pass filter, with variable Q. */
	void makeBandPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>);

	/** Creates the coefficients for a notch filter, with variable Q. */
	void makeNotch (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>);

	/** Creates the coefficients for an all-pass filter, with variable Q. */
	void makeAllPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>);

	//==============================================================================
	/** Creates the coefficients for a low-pass shelf filter with variable Q and gain.
		The gain is a scale factor that the low frequencies are multiplied by, so values greater than 1.0 will boost the low frequencies, values less than 1.0 will attenuate them.
	*/
	void makeLowShelf (double	  sampleRate,
					   Sampletype cutOffFrequency,
					   Sampletype Q			 = inverseRootTwo<Sampletype>,
					   Sampletype gainFactor = Sampletype (1.));

	/** Creates the coefficients for a high-pass shelf filter with variable Q and gain.
		The gain is a scale factor that the high frequencies are multiplied by, so values greater than 1.0 will boost the high frequencies, values less than 1.0 will attenuate them.
	*/
	void makeHighShelf (double	   sampleRate,
						Sampletype cutOffFrequency,
						Sampletype Q		  = inverseRootTwo<Sampletype>,
						Sampletype gainFactor = Sampletype (1.));

	/** Creates the coefficients for a peak filter centred around a given frequency, with a variable Q and gain.
		The gain is a scale factor that the centre frequencies are multiplied by, so values greater than 1.0 will boost the centre frequencies, values less than 1.0 will attenuate them.
	*/
	void makePeakFilter (double		sampleRate,
						 Sampletype centreFrequency,
						 Sampletype Q		   = inverseRootTwo<Sampletype>,
						 Sampletype gainFactor = Sampletype (1.));

	//==============================================================================
	/** Returns the filter order associated with the coefficients */
	int getFilterOrder() const noexcept;

	//==============================================================================
	/** The raw coefficients.
		You should leave these numbers alone unless you really know what you're doing.
	*/

	Sampletype*		  getRawCoefficients() noexcept;
	const Sampletype* getRawCoefficients() const noexcept;

	struct LIMES_EXPORT Storage final
	{
		Storage();

		scalar_vector<Sampletype> coefficients;

		scalar_vector<Sampletype>* operator->() noexcept;

		const scalar_vector<Sampletype>* operator->() const noexcept;

		Storage& operator= (std::initializer_list<Sampletype> list);
	};

	Storage coefficients;
};

}  // namespace dsp::filters

LIMES_END_NAMESPACE
