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

// IWYU pragma: private
// IWYU pragma: friend fallback_resampler.h

#pragma once

#include <limes_namespace.h>
#include <limes_export.h>
#include <limes_core.h>
#include <functional>

/** @file
	This file contains the Speex resampling code.

	@ingroup resampling

	The original license:

	Copyright (C) 2007 Jean-Marc Valin

	File: speex_resampler.h
	Resampling code

	The design goals of this code are:
		- Very fast algorithm
		- Low memory requirement
		- Good *perceptual* quality (and not best SNR)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are
	met:
	1. Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains the code of the Speex resampling library.
	@ingroup resampling
 */
namespace speex
{

/** A resampler class based on the Speex library's implementation.
 */
template <typename SampleType>
class LIMES_NO_EXPORT Resampler final
{
public:
	/** @name Constructors */
	///@{

	/** Create a new resampler with integer input and output rates.

		@param nb_channels Number of channels to be processed
		@param in_rate Input sampling rate (integer number of Hz).
		@param out_rate Output sampling rate (integer number of Hz).
		@param quality Resampling quality between 0 and 10, where 0 has poor quality
		and 10 has very high quality.
	 */
	Resampler (std::uint32_t numChannels,
			   std::uint32_t inRate,
			   std::uint32_t outRate,
			   int			 qualityToUse);

	/** Create a new resampler with fractional input/output rates. The sampling
		rate ratio is an arbitrary rational number with both the numerator and
		denominator being 32-bit integers.

		@param numChannels Number of channels to be processed
		@param ratio_num Numerator of the sampling rate ratio
		@param ratio_den Denominator of the sampling rate ratio
		@param in_rate Input sampling rate rounded to the nearest integer (in Hz).
		@param out_rate Output sampling rate rounded to the nearest integer (in Hz).
		@param quality Resampling quality between 0 and 10, where 0 has poor quality
		and 10 has very high quality.

		@throws std::runtime_error An exception will be thrown if the resampler cannot be
		correctly constructed (for example, if invalid arguments were supplied).
	 */
	Resampler (std::uint32_t numChannels,
			   std::uint32_t ratio_num,
			   std::uint32_t ratio_den,
			   std::uint32_t inRate,
			   std::uint32_t outRate,
			   int			 qualityToUse);
	///@}

	LIMES_NON_COPYABLE (Resampler)
	LIMES_DEFAULT_MOVABLE (Resampler)

	/** @name Processing */
	///@{
	/** Resample a float array. The input and output buffers must *not* overlap.

		@param channel_index Index of the channel to process for the multi-channel
		base (0 otherwise)
		@param in Input buffer
		@param in_len Number of input samples in the input buffer. Returns the
		number of samples processed
		@param out Output buffer
		@param out_len Size of the output buffer. Returns the number of samples written
	 */
	void process (std::size_t		channel_index,
				  const SampleType* in,
				  std::uint32_t&	in_len,
				  SampleType*		out,
				  std::uint32_t&	out_len) noexcept;

	/** Resample an interleaved float array. The input and output buffers must *not* overlap.

		@param in Input buffer
		@param in_len Number of input samples in the input buffer. Returns the number
		of samples processed. This is all per-channel.
		@param out Output buffer
		@param out_len Size of the output buffer. Returns the number of samples written.
		This is all per-channel.
	 */
	void process_interleaved (const SampleType* in,
							  std::uint32_t&	in_len,
							  SampleType*		out,
							  std::uint32_t&	out_len) noexcept;
	///@}

	/** @name Setting the resampling rate/ratio */
	///@{
	/** Set (change) the input/output sampling rates (integer value).
		@param in_rate Input sampling rate (integer number of Hz).
		@param out_rate Output sampling rate (integer number of Hz).
	 */
	void set_rate (std::uint32_t in_rate,
				   std::uint32_t out_rate) noexcept;

	/** Set (change) the input/output sampling rates and resampling ratio
		(fractional values in Hz supported).

		@param ratio_num Numerator of the sampling rate ratio
		@param ratio_den Denominator of the sampling rate ratio
		@param inRate Input sampling rate rounded to the nearest integer (in Hz).
		@param outRate Output sampling rate rounded to the nearest integer (in Hz).
	 */
	void set_rate_frac (std::uint32_t ratio_num,
						std::uint32_t ratio_den,
						std::uint32_t inRate,
						std::uint32_t outRate) noexcept;
	///@}

	/** @name Retrieving the resampling rate & ratio */
	///@{
	/** Get the current input/output sampling rates (integer value).
		@param inRate Input sampling rate (integer number of Hz) copied.
		@param outRate Output sampling rate (integer number of Hz) copied.
	 */
	void get_rate (std::uint32_t& inRate,
				   std::uint32_t& outRate) const noexcept;

	/** Get the current resampling ratio. This will be reduced to the least
		common denominator.
		@param ratio_num Numerator of the sampling rate ratio copied
		@param ratio_den Denominator of the sampling rate ratio copied
	 */
	void get_ratio (std::uint32_t& ratio_num,
					std::uint32_t& ratio_den) const noexcept;
	///@}

	/** @name Quality */
	///@{
	/** Set (change) the quality.
		@param quality The new quality to use, 0-10.

		@throws std::runtime_error An exception will be thrown if \c newQuality is
		out of the valid range of quality values.
	 */
	void set_quality (int newQuality);

	/** Returns the current quality. */
	int get_quality() const noexcept;
	///@}

	/** @name Input stride */
	///@{
	/** Set (change) the input stride.
		@param stride Input stride
	 */
	void set_input_stride (std::uint32_t stride) noexcept;

	/** Get the input stride. */
	std::uint32_t get_input_stride() const noexcept;
	///@}

	/** @name Output stride */
	///@{
	/** Set (change) the output stride.
		@param stride Output stride
	 */
	void set_output_stride (std::uint32_t stride) noexcept;

	/** Get the output stride. */
	std::uint32_t get_output_stride() const noexcept;
	///@}

	/** @name Latency reporting */
	///@{
	/** Get the latency in input samples introduced by the resampler. */
	int get_input_latency() const noexcept;

	/** Get the latency in output samples introduced by the resampler. */
	int get_output_latency() const noexcept;
	///@}

	/** Make sure that the first samples to go out of the resamplers don't have
		leading zeros. This is only useful before starting to use a newly created
		resampler. It is recommended to use that when resampling an audio file, as
		it will generate a file with the same length. For real-time processing,
		it is probably easier not to use this call (so that the output duration
		is the same for the first frame).
	 */
	void skip_zeroes() noexcept;

	/** Resets the resampler so a new (unrelated) stream can be processed. */
	void reset_mem();

	/** Returns the number of channels this resampler has been prepared to process. */
	std::uint32_t get_num_channels() const noexcept;

private:
	void update_filter() noexcept;

	int resample_basic_direct (std::uint32_t channel_index, const SampleType* in, std::uint32_t in_len, SampleType* out, std::uint32_t out_len) noexcept;
	int resample_basic_interpolate (std::uint32_t channel_index, const SampleType* in, std::uint32_t in_len, SampleType* out, std::uint32_t out_len) noexcept;

	std::uint32_t in_rate { 0 };
	std::uint32_t out_rate { 0 };
	std::uint32_t num_rate { 0 };
	std::uint32_t den_rate { 0 };

	int			  quality { -1 };
	std::uint32_t nb_channels { 0 };
	std::uint32_t filt_len { 0 };
	int			  int_advance { 0 };
	int			  frac_advance { 0 };
	SampleType	  cutoff { 1 };
	std::uint32_t oversample { 0 };
	int			  started { 0 };

	/* These are per-channel */
	memory::array_pointer<std::int32_t>	 last_sample { nb_channels };
	memory::array_pointer<std::uint32_t> samp_frac_num { nb_channels };
	memory::array_pointer<std::uint32_t> magic_samples { nb_channels };

	memory::array_pointer<SampleType, false> mem, sinc_table;

	bool use_interp_func { true };

	unsigned in_stride { 1 };
	unsigned out_stride { 1 };
};

}  // namespace speex

LIMES_END_NAMESPACE
