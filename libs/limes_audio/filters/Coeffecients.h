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
#include "../util/AudioBuffer.h"
#include <limes_namespace.h>
#include <limes_core.h>

/** @defgroup dsp_filters Filters
	Classes for filtering realtime audio signals.
	@ingroup limes_audio
 */

/** @dir libs/limes_audio/filters
	This directory contains classes for filtering realtime audio signals.
	@ingroup dsp_filters
 */

/** @file
	This file defines the Coeffecients class.
	@ingroup dsp_filters
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains classes for filtering realtime audio signals.
	@ingroup dsp_filters
 */
namespace dsp::filters
{

/** A filter coefficients class, with lots of methods for creating various filter topologies.
	This class is designed so that coefficients can be recreated and reassigned during realtime playback; no allocations should occur.
	@ingroup dsp_filters
 */
template <Sample Sampletype>
class LIMES_EXPORT Coeffecients final
{
public:

	/** @name First-order filter creation */
	///@{
	/** Creates the coefficients for a first order low pass filter. */
	void makeFirstOrderLowPass (double sampleRate, Sampletype frequency) noexcept;

	/** Creates the coefficients for a first order high pass filter. */
	void makeFirstOrderHighPass (double sampleRate, Sampletype frequency) noexcept;

	/** Creates the coefficients for a first order all-pass filter. */
	void makeFirstOrderAllPass (double sampleRate, Sampletype frequency) noexcept;
	///@}

	/** @name Filter creation with variable Q */
	///@{
	/** Creates the coefficients for a low pass filter, with variable Q. */
	void makeLowPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>) noexcept;

	/** Creates the coefficients for a high pass filter, with variable Q. */
	void makeHighPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>) noexcept;

	/** Creates the coefficients for a band pass filter, with variable Q. */
	void makeBandPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>) noexcept;

	/** Creates the coefficients for a notch filter, with variable Q. */
	void makeNotch (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>) noexcept;

	/** Creates the coefficients for an all-pass filter, with variable Q. */
	void makeAllPass (double sampleRate, Sampletype frequency, Sampletype Q = inverseRootTwo<Sampletype>) noexcept;
	///@}

	/** @name Filter creation with variable Q and gain
		The gain is a scale factor that the low frequencies are multiplied by, so values greater than 1.0 will boost the low frequencies, values less than 1.0 will attenuate them.
	 */
	///@{
	/** Creates the coefficients for a low-pass shelf filter with variable Q and gain. */
	void makeLowShelf (double	  sampleRate,
					   Sampletype cutOffFrequency,
					   Sampletype Q			 = inverseRootTwo<Sampletype>,
					   Sampletype gainFactor = Sampletype (1.)) noexcept;

	/** Creates the coefficients for a high-pass shelf filter with variable Q and gain. */
	void makeHighShelf (double	   sampleRate,
						Sampletype cutOffFrequency,
						Sampletype Q		  = inverseRootTwo<Sampletype>,
						Sampletype gainFactor = Sampletype (1.)) noexcept;

	/** Creates the coefficients for a peak filter centred around a given frequency, with a variable Q and gain. */
	void makePeakFilter (double		sampleRate,
						 Sampletype centreFrequency,
						 Sampletype Q		   = inverseRootTwo<Sampletype>,
						 Sampletype gainFactor = Sampletype (1.)) noexcept;
	///@}

	/** Returns the filter order associated with the coefficients. */
	std::size_t getFilterOrder() const noexcept;

	/** @name Raw coefficient access */
	///@{
	/** The raw coefficients.
		@warning You should leave these numbers alone unless you really know what you're doing.
	 */
	Sampletype*		  getRawCoefficients() noexcept;
	const Sampletype* getRawCoefficients() const noexcept;
	///@}

	/** A simple wrapper around a vector, to be used to hold the actual filter coeffecients.
		This class is designed to be assignable with \c operator= without allocating.
	 */
	struct LIMES_EXPORT Storage final
	{
		Storage();

		LIMES_DEFAULT_MOVABLE (Storage)
		LIMES_DEFAULT_COPYABLE (Storage)

		std::vector<Sampletype>* operator->() noexcept;

		const std::vector<Sampletype>* operator->() const noexcept;

		Storage& operator= (std::initializer_list<Sampletype> list) noexcept;

		/** The actual storage. */
		std::vector<Sampletype> coefficients;
	};

	/** The raw coefficients.
		@warning You should leave these numbers alone unless you really know what you're doing.
	 */
	Storage coefficients;
};

}  // namespace dsp::filters

LIMES_END_NAMESPACE
