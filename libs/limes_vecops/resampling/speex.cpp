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

#include <limes_export.h>
#include <limes_namespace.h>
#include "./speex.h"

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <array>

LIMES_BEGIN_NAMESPACE

namespace speex
{

static constexpr std::array<double, 68> kaiser12_table = {
	0.99859849, 1.00000000, 0.99859849, 0.99440475, 0.98745105, 0.97779076,
	0.96549770, 0.95066529, 0.93340547, 0.91384741, 0.89213598, 0.86843014,
	0.84290116, 0.81573067, 0.78710866, 0.75723148, 0.72629970, 0.69451601,
	0.66208321, 0.62920216, 0.59606986, 0.56287762, 0.52980938, 0.49704014,
	0.46473455, 0.43304576, 0.40211431, 0.37206735, 0.34301800, 0.31506490,
	0.28829195, 0.26276832, 0.23854851, 0.21567274, 0.19416736, 0.17404546,
	0.15530766, 0.13794294, 0.12192957, 0.10723616, 0.09382272, 0.08164178,
	0.07063950, 0.06075685, 0.05193064, 0.04409466, 0.03718069, 0.03111947,
	0.02584161, 0.02127838, 0.01736250, 0.01402878, 0.01121463, 0.00886058,
	0.00691064, 0.00531256, 0.00401805, 0.00298291, 0.00216702, 0.00153438,
	0.00105297, 0.00069463, 0.00043489, 0.00025272, 0.00013031, 0.0000527734,
	0.00001000, 0.00000000
};

static constexpr std::array<double, 36> kaiser10_table = {
	0.99537781, 1.00000000, 0.99537781, 0.98162644, 0.95908712, 0.92831446,
	0.89005583, 0.84522401, 0.79486424, 0.74011713, 0.68217934, 0.62226347,
	0.56155915, 0.50119680, 0.44221549, 0.38553619, 0.33194107, 0.28205962,
	0.23636152, 0.19515633, 0.15859932, 0.12670280, 0.09935205, 0.07632451,
	0.05731132, 0.04193980, 0.02979584, 0.02044510, 0.01345224, 0.00839739,
	0.00488951, 0.00257636, 0.00115101, 0.00035515, 0.00000000, 0.00000000
};

static constexpr std::array<double, 36> kaiser8_table = {
	0.99635258, 1.00000000, 0.99635258, 0.98548012, 0.96759014, 0.94302200,
	0.91223751, 0.87580811, 0.83439927, 0.78875245, 0.73966538, 0.68797126,
	0.63451750, 0.58014482, 0.52566725, 0.47185369, 0.41941150, 0.36897272,
	0.32108304, 0.27619388, 0.23465776, 0.19672670, 0.16255380, 0.13219758,
	0.10562887, 0.08273982, 0.06335451, 0.04724088, 0.03412321, 0.02369490,
	0.01563093, 0.00959968, 0.00527363, 0.00233883, 0.00050000, 0.00000000
};

static constexpr std::array<double, 36> kaiser6_table = {
	0.99733006, 1.00000000, 0.99733006, 0.98935595, 0.97618418, 0.95799003,
	0.93501423, 0.90755855, 0.87598009, 0.84068475, 0.80211977, 0.76076565,
	0.71712752, 0.67172623, 0.62508937, 0.57774224, 0.53019925, 0.48295561,
	0.43647969, 0.39120616, 0.34752997, 0.30580127, 0.26632152, 0.22934058,
	0.19505503, 0.16360756, 0.13508755, 0.10953262, 0.08693120, 0.06722600,
	0.05031820, 0.03607231, 0.02432151, 0.01487334, 0.00752000, 0.00000000
};

struct LIMES_NO_EXPORT FuncDef final
{
	const double* table;
	int			  oversample;
};

static constexpr FuncDef KAISER12 = { kaiser12_table.data(), 64 };

static constexpr FuncDef KAISER10 = { kaiser10_table.data(), 32 };

static constexpr FuncDef KAISER8 = { kaiser8_table.data(), 32 };

static constexpr FuncDef KAISER6 = { kaiser6_table.data(), 32 };


/* This table maps conversion quality to internal parameters. There are two
	reasons that explain why the up-sampling bandwidth is larger than the
	down-sampling bandwidth:
	1) When up-sampling, we can assume that the spectrum is already attenuated
	close to the Nyquist rate (from an A/D or a previous resampling filter)
	2) Any aliasing that occurs very close to the Nyquist rate will be masked
	by the sinusoids/noise just below the Nyquist rate (guaranteed only for
	up-sampling).
*/

struct LIMES_NO_EXPORT QualityMapping final
{
	int	  base_length;
	int	  oversample;
	float downsample_bandwidth;
	float upsample_bandwidth;

	const FuncDef* window_func;
};

using QualityMap = std::array<QualityMapping, 11>;

static constexpr QualityMap quality_map = {
	QualityMapping { 8, 4, 0.83f, 0.86f, &KAISER6 },	   /* Q0 */
	QualityMapping { 16, 4, 0.85f, 0.88f, &KAISER6 },	   /* Q1 */
	QualityMapping { 32, 4, 0.882f, 0.91f, &KAISER6 },	   /* Q2 - 82.3% cutoff ( ~60 dB stop) 6  */
	QualityMapping { 48, 8, 0.895f, 0.917f, &KAISER8 },	   /* Q3 - 84.9% cutoff ( ~80 dB stop) 8  */
	QualityMapping { 64, 8, 0.921f, 0.94f, &KAISER8 },	   /* Q4 - 88.7% cutoff ( ~80 dB stop) 8  */
	QualityMapping { 80, 16, 0.922f, 0.94f, &KAISER10 },   /* Q5 - 89.1% cutoff (~100 dB stop) 10 */
	QualityMapping { 96, 16, 0.94f, 0.945f, &KAISER10 },   /* Q6 - 91.5% cutoff (~100 dB stop) 10 */
	QualityMapping { 128, 16, 0.95f, 0.95f, &KAISER10 },   /* Q7 - 93.1% cutoff (~100 dB stop) 10 */
	QualityMapping { 160, 16, 0.96f, 0.96f, &KAISER10 },   /* Q8 - 94.5% cutoff (~100 dB stop) 10 */
	QualityMapping { 192, 32, 0.968f, 0.968f, &KAISER12 }, /* Q9 - 95.5% cutoff (~100 dB stop) 10 */
	QualityMapping { 256, 32, 0.975f, 0.975f, &KAISER12 }, /* Q10 - 96.6% cutoff (~100 dB stop) 10 */
};


/* The slow way of computing a sinc for the table. Should improve that some day */
[[nodiscard]] static inline float sinc (float cutoff, float x, int N, const FuncDef* window_func) noexcept
{
	const auto xx = x * cutoff;

	const auto fabsVal = fabsf (x);

	if (fabsVal < 1e-6)
		return cutoff;

	if (fabsVal > .5f * N)
		return 0.f;

	const auto computedVal = [X = std::fabs (2.f * x / N), window_func]
	{
		const auto y	= X * window_func->oversample;
		const auto ind	= math::round (std::floor (y));
		const auto frac = (y - ind);

		double interp[4];

		const auto fracSqr	= frac * frac;
		const auto fracCube = fracSqr * frac;

		/* CSE with handle the repeated powers */
		interp[3] = -0.1666666667 * frac + 0.1666666667 * fracCube;
		interp[2] = frac + 0.5 * fracSqr - 0.5 * fracCube;
		interp[0] = -0.3333333333 * frac + 0.5 * fracSqr - 0.1666666667 * fracCube;

		/* Just to make sure we don't have rounding problems */
		interp[1] = 1. - interp[3] - interp[2] - interp[0];

		/* sum = frac*accum[1] + (1-frac)*accum[2]; */
		return interp[0]
				 * window_func->table[ind]
			 + interp[1]
				   * window_func->table[ind + 1]
			 + interp[2]
				   * window_func->table[ind + 2]
			 + interp[3]
				   * window_func->table[ind + 3];
	}();

	/* FIXME: Can it really be any slower than this? */
	return cutoff
		 * std::sin (math::constants::pi<float> * xx)
		 / (math::constants::pi<float> * xx) * static_cast<float> (computedVal);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

Resampler::Resampler (std::uint32_t numChannels,
					  std::uint32_t inRate,
					  std::uint32_t outRate,
					  int			qualityToUse)
	: Resampler (numChannels, inRate, outRate, inRate, outRate, qualityToUse)
{
}

Resampler::Resampler (std::uint32_t numChannels,
					  std::uint32_t ratio_num,
					  std::uint32_t ratio_den,
					  std::uint32_t inRate,
					  std::uint32_t outRate,
					  int			qualityToUse)
	: nb_channels (numChannels)
{
	for (unsigned i = 0; i < nb_channels; ++i)
	{
		last_sample[i]	 = 0;
		magic_samples[i] = 0;
		samp_frac_num[i] = 0;
	}

	set_quality (qualityToUse);

	set_rate_frac (ratio_num, ratio_den, inRate, outRate);

	update_filter();

	initialised = 1;
}

void Resampler::process (std::size_t	channel_index,
						 const float*	in,
						 std::uint32_t* in_len,
						 float*			out,
						 std::uint32_t* out_len) noexcept
{
	const auto N = filt_len;

	auto* const localMem = mem + channel_index * mem_alloc_size;

	unsigned tmp_out_len = 0;

	started = 1;

	// Handle the case where we have samples left from a reduction in filter length
	if (magic_samples[channel_index] > 0)
	{
		auto tmp_in_len = magic_samples[channel_index];
		tmp_out_len		= *out_len;

		// magic_samples needs to be set to zero to avoid infinite recursion
		const auto tmp_magic		 = magic_samples[channel_index];
		magic_samples[channel_index] = 0;

		const auto istride_save = in_stride;
		in_stride				= 1;

		process (channel_index, localMem + N - 1, &tmp_in_len, out, &tmp_out_len);
		in_stride = istride_save;

		// If we couldn't process all "magic" input samples, save the rest for next time
		if (tmp_in_len < tmp_magic)
		{
			magic_samples[channel_index] = tmp_magic - tmp_in_len;

			for (unsigned i = 0; i < magic_samples[channel_index]; ++i)
				localMem[N - 1 + i] = localMem[N - 1 + i + tmp_in_len];
		}

		out += tmp_out_len * out_stride;
		*out_len -= tmp_out_len;
	}

	/* Call the right resampler through the function ptr */
	const auto out_sample = resampler_ptr (static_cast<std::uint32_t> (channel_index), in, *in_len, out, *out_len);

	if (last_sample[channel_index] < static_cast<int> (*in_len))
		*in_len = static_cast<std::uint32_t> (last_sample[channel_index]);

	*out_len = static_cast<std::uint32_t> (static_cast<unsigned> (out_sample) + tmp_out_len);

	last_sample[channel_index] -= *in_len;

	std::uint32_t j = 0;

	for (; j < N - 1 - static_cast<std::uint32_t> (*in_len); ++j)
		localMem[j] = localMem[j + *in_len];

	if (in == nullptr)
		for (; j < N - 1; ++j) localMem[j] = 0;
	else
		for (; j < N - 1; ++j) localMem[j] = in[in_stride * (j + *in_len - N + 1)];
}

void Resampler::process_interleaved (const float*	in,
									 std::uint32_t* in_len,
									 float*			out,
									 std::uint32_t* out_len) noexcept
{
	const auto bak_len = *out_len;

	const auto istride_save = in_stride;
	const auto ostride_save = out_stride;

	in_stride = out_stride = nb_channels;

	for (unsigned i = 0; i < nb_channels; ++i)
	{
		*out_len = bak_len;

		if (in == nullptr)
			process (i, nullptr, in_len, out + i, out_len);
		else
			process (i, in + i, in_len, out + i, out_len);
	}

	in_stride  = istride_save;
	out_stride = ostride_save;
}

void Resampler::update_filter()
{
	const auto old_length = filt_len;
	const auto qualParams = quality_map[static_cast<QualityMap::size_type> (quality)];

	oversample = static_cast<std::uint32_t> (qualParams.oversample);
	filt_len   = static_cast<std::uint32_t> (qualParams.base_length);

	if (num_rate > den_rate)  // downsampling
	{
		cutoff = qualParams.downsample_bandwidth * den_rate / num_rate;

		filt_len = static_cast<unsigned> (math::round (std::ceil (filt_len * (static_cast<double> (num_rate) / static_cast<double> (den_rate)))));

		/* Round down to make sure we have a multiple of 4 */
		filt_len &= (~std::uint32_t (0x3));

		if (2 * den_rate < num_rate)
			oversample >>= 1;

		if (4 * den_rate < num_rate)
			oversample >>= 1;

		if (8 * den_rate < num_rate)
			oversample >>= 1;

		if (16 * den_rate < num_rate)
			oversample >>= 1;

		if (oversample < 1)
			oversample = 1;
	}
	else  // upsampling
	{
		cutoff = qualParams.upsample_bandwidth;
	}

	/* Choose the resampling type that requires the least amount of memory */
	resampler_ptr = [this, &qualParams]() -> ResampleFunc
	{
		if (den_rate <= oversample)
		{
			sinc_table.reallocate (filt_len * den_rate);

			for (unsigned i = 0; i < den_rate; ++i)
			{
				for (unsigned j = 0; j < filt_len; ++j)
				{
					sinc_table[static_cast<int> (i * filt_len + j)] = sinc (cutoff,
																			static_cast<float> ((static_cast<int> (j) - static_cast<int> (filt_len) / 2 + 1) - (static_cast<float> (i)) / den_rate),
																			static_cast<int> (filt_len),
																			qualParams.window_func);
				}
			}

			if (quality > 8)
				return [this] (std::uint32_t channel_index, const float* in, std::uint32_t in_len, float* out, std::uint32_t out_len)
				{
					return resample_basic_direct<double> (channel_index, in, in_len, out, out_len);
				};

			return [this] (std::uint32_t channel_index, const float* in, std::uint32_t in_len, float* out, std::uint32_t out_len)
			{
				return resample_basic_direct<float> (channel_index, in, in_len, out, out_len);
			};
		}

		sinc_table.reallocate (filt_len * oversample + 8);

		for (int i = -4; i < static_cast<int> (oversample * filt_len + 4); ++i)
		{
			sinc_table[i + 4] = sinc (cutoff,
									  static_cast<float> (i / static_cast<float> (oversample) - filt_len / 2),
									  static_cast<int> (filt_len),
									  qualParams.window_func);
		}

		if (quality > 8)
			return [this] (std::uint32_t channel_index, const float* in, std::uint32_t in_len, float* out, std::uint32_t out_len)
			{
				return resample_basic_interpolate<double> (channel_index, in, in_len, out, out_len);
			};

		return [this] (std::uint32_t channel_index, const float* in, std::uint32_t in_len, float* out, std::uint32_t out_len)
		{
			return resample_basic_interpolate<float> (channel_index, in, in_len, out, out_len);
		};
	}();

	int_advance	 = static_cast<int> (num_rate / den_rate);
	frac_advance = static_cast<int> (num_rate % den_rate);

	/* Here's the place where we update the filter memory to take into
	   account the change in filter length. It's probably the messiest
	   part of the code due to handling of lots of corner cases. */

	if (mem == nullptr || ! started)
	{
		mem.reallocate (nb_channels * (filt_len - 1));

		memset (&mem[0], 0, nb_channels * (filt_len - 1));

		mem_alloc_size = filt_len - 1;

		return;
	}

	if (filt_len == old_length)
		return;

	if (filt_len < old_length)
	{
		/* Reduce filter length, this a bit tricky. We need to store
		   some of the memory as "magic" samples so they can be used
		   directly as input the next time(s) */

		for (unsigned i = 0; i < nb_channels; ++i)
		{
			const auto old_magic = magic_samples[i];

			magic_samples[i] = (old_length - filt_len) / 2;

			/* We must copy some of the memory that's no longer used */
			/* Copy data going backward */

			for (unsigned j = 0; j < filt_len - 1 + magic_samples[i] + old_magic; ++j)
			{
				mem[i * mem_alloc_size + j] = mem[i * mem_alloc_size + j + magic_samples[i]];
			}

			magic_samples[i] += old_magic;
		}

		return;
	}

	/* Increase the filter length */

	const auto old_alloc_size = mem_alloc_size;

	if (filt_len - 1 > mem_alloc_size)
	{
		mem.reallocate (nb_channels * (filt_len - 1));

		mem_alloc_size = filt_len - 1;
	}

	for (auto i = nb_channels - 1; i >= 0; --i)
	{
		unsigned j;
		auto	 olen = old_length;

		/*if (st->magic_samples[i])*/
		{
			/* Try and remove the magic samples as if nothing had happened */

			/* FIXME: This is wrong but for now we need it to
			 * avoid going over the array bounds */

			olen = old_length + 2 * magic_samples[i];

			for (j = old_length - 2 + magic_samples[i]; j >= 0; --j)
			{
				mem[i * mem_alloc_size + j + magic_samples[i]] = mem[i * old_alloc_size + j];
			}

			for (j = 0; j < magic_samples[i]; ++j)
			{
				mem[i * mem_alloc_size + j] = 0;
			}

			magic_samples[i] = 0;
		}

		if (filt_len > olen)
		{
			/* If the new filter length is still bigger than the "augmented" length */
			/* Copy data going backward */

			for (j = 0; j < olen - 1; ++j)
			{
				mem[i * mem_alloc_size + (filt_len - 2 - j)] = mem[i * mem_alloc_size + (olen - 2 - j)];
			}

			/* Then put zeros for lack of anything better */
			for (; j < filt_len - 1; ++j)
			{
				mem[i * mem_alloc_size + (filt_len - 2 - j)] = 0;
			}

			/* Adjust last_sample */
			last_sample[i] += (filt_len - olen) / 2;
		}
		else
		{
			/* Put back some of the magic! */
			magic_samples[i] = (olen - filt_len) / 2;

			for (j = 0; j < filt_len - 1 + magic_samples[i]; ++j)
			{
				mem[i * mem_alloc_size + j] = mem[i * mem_alloc_size + j + magic_samples[i]];
			}
		}
	}
}

template <typename Type>
int Resampler::resample_basic_direct (std::uint32_t channel_index, const float* in, std::uint32_t in_len, float* out, std::uint32_t out_len) noexcept
{
	const auto		  N			  = filt_len;
	int				  out_sample  = 0;
	auto			  lastSample  = static_cast<unsigned> (last_sample[channel_index]);
	auto			  sampFracNum = samp_frac_num[channel_index];
	const auto* const localMem	  = mem + channel_index * mem_alloc_size;

	while (! (lastSample >= in_len || out_sample >= static_cast<int> (out_len)))
	{
		unsigned j	 = 0;
		Type	 sum = 0.;

		for (; lastSample - N + 1 + j < 0; ++j)
		{
			const auto tableVal = static_cast<float> (sinc_table[sampFracNum * filt_len + j]);

			sum += static_cast<Type> (localMem[lastSample + j] * tableVal);
		}

		/* Do the new part */
		if (in != nullptr)
		{
			const auto* ptr = in + in_stride * (lastSample - N + 1 + j);

			for (; j < N; ++j)
			{
				const auto tableVal = static_cast<float> (sinc_table[static_cast<int> (sampFracNum * filt_len + j)]);

				sum += static_cast<Type> (*ptr * tableVal);
				ptr += in_stride;
			}
		}

		*out = static_cast<float> (sum);

		out += out_stride;
		out_sample++;

		lastSample += static_cast<unsigned> (int_advance);
		sampFracNum += static_cast<std::uint32_t> (frac_advance);

		if (sampFracNum >= den_rate)
		{
			sampFracNum -= den_rate;
			lastSample++;
		}
	}

	last_sample[channel_index]	 = static_cast<std::int32_t> (lastSample);
	samp_frac_num[channel_index] = sampFracNum;

	return out_sample;
}

template int Resampler::resample_basic_direct<double> (std::uint32_t, const float*, std::uint32_t, float*, std::uint32_t) noexcept;
template int Resampler::resample_basic_direct<float> (std::uint32_t, const float*, std::uint32_t, float*, std::uint32_t) noexcept;

template <typename Type>
int Resampler::resample_basic_interpolate (std::uint32_t channel_index, const float* in, std::uint32_t in_len, float* out, std::uint32_t out_len) noexcept
{
	const auto		  N			  = filt_len;
	int				  out_sample  = 0;
	auto			  lastSample  = static_cast<unsigned> (last_sample[channel_index]);
	auto			  sampFracNum = samp_frac_num[channel_index];
	const auto* const localMem	  = mem + channel_index * mem_alloc_size;

	float accum[4] = { 0.f, 0.f, 0.f, 0.f };
	float interp[4];

	while (! (lastSample >= in_len || out_sample >= static_cast<int> (out_len)))
	{
		const auto alpha  = static_cast<float> (sampFracNum) / den_rate;
		const auto offset = sampFracNum * oversample / den_rate;
		const auto frac	  = alpha * oversample - offset;

		/* This code is written like this to make it easy to optimise
		 * with SIMD.  For most DSPs, it would be best to split the
		 * loops in two because most DSPs have only two
		 * accumulators */

		unsigned j = 0;

		for (; lastSample - N + 1 + j < 0; ++j)
		{
			const auto curr_mem = static_cast<float> (localMem[lastSample + j]);

			const auto idx = static_cast<int> (4 + (j + 1) * oversample - offset);

			for (auto i = -2, k = 0; k < 4; ++k, ++i)
				accum[k] += curr_mem * sinc_table[idx + i];
		}

		if (in != nullptr)
		{
			const auto* ptr = in + in_stride * (lastSample - N + 1 + j);

			/* Do the new part */
			for (; j < N; ++j)
			{
				const auto curr_in = static_cast<float> (*ptr);

				ptr += in_stride;

				const auto idx = static_cast<int> (4 + (j + 1) * oversample - offset);

				for (auto i = -2, k = 0; k < 4; ++k, ++i)
					accum[k] += curr_in * sinc_table[idx + i];
			}
		}

		/* Compute interpolation coefficients. I'm not sure whether this
		corresponds to cubic interpolation but I know it's MMSE-optimal on
		a sinc */

		const auto fracSqr	= frac * frac;
		const auto fracCube = fracSqr * frac;

		interp[0] = -0.16667f * frac + 0.16667f * fracCube;
		interp[1] = frac + 0.5f * fracSqr - 0.5f * fracCube;
		interp[3] = -0.33333f * frac + 0.5f * fracSqr - 0.16667f * fracCube;

		/* Just to make sure we don't have rounding problems */
		interp[2] = 1.f - interp[0] - interp[1] - interp[3];

		*out = interp[0] * accum[0] + interp[1] * accum[1] + interp[2] * accum[2] + interp[3] * accum[3];

		out += out_stride;
		out_sample++;

		lastSample += static_cast<unsigned> (int_advance);
		sampFracNum += static_cast<std::uint32_t> (frac_advance);

		if (sampFracNum >= den_rate)
		{
			sampFracNum -= den_rate;
			lastSample++;
		}
	}

	last_sample[channel_index]	 = static_cast<std::int32_t> (lastSample);
	samp_frac_num[channel_index] = sampFracNum;

	return out_sample;
}

template int Resampler::resample_basic_interpolate<double> (std::uint32_t, const float*, std::uint32_t, float*, std::uint32_t) noexcept;
template int Resampler::resample_basic_interpolate<float> (std::uint32_t, const float*, std::uint32_t, float*, std::uint32_t) noexcept;

void Resampler::set_rate (std::uint32_t inRate,
						  std::uint32_t outRate) noexcept
{
	set_rate_frac (inRate, outRate, inRate, outRate);
}

void Resampler::set_rate_frac (std::uint32_t ratio_num,
							   std::uint32_t ratio_den,
							   std::uint32_t inRate,
							   std::uint32_t outRate) noexcept
{
	if (in_rate == inRate && out_rate == outRate && num_rate == ratio_num && den_rate == ratio_den)
		return;

	const auto old_den = den_rate;

	in_rate	 = inRate;
	out_rate = outRate;
	num_rate = ratio_num;
	den_rate = ratio_den;

	{
		const auto g = [nr = num_rate, dr = den_rate]
		{
			auto a = nr;
			auto b = dr;

			while (b > 0)
			{
				const auto tmp = b;

				b = a % b;
				a = tmp;
			}

			return a;
		}();

		num_rate /= g;
		den_rate /= g;
	}

	if (old_den > 0)
	{
		const auto changeRatio = den_rate / old_den;

		for (unsigned i = 0; i < nb_channels; ++i)
		{
			samp_frac_num[i] *= changeRatio;

			if (samp_frac_num[i] >= den_rate)
				samp_frac_num[i] = den_rate - 1;
		}
	}

	if (initialised)
		update_filter();
}

void Resampler::get_rate (std::uint32_t& inRate,
						  std::uint32_t& outRate) const noexcept
{
	inRate	= in_rate;
	outRate = out_rate;
}

void Resampler::get_ratio (std::uint32_t& ratio_num,
						   std::uint32_t& ratio_den) const noexcept
{
	ratio_num = num_rate;
	ratio_den = den_rate;
}

void Resampler::set_quality (int newQuality)
{
	if (newQuality > 10 || newQuality < 0)
	{
		std::stringstream s;

		s << "Speex resampler: Invalid quality argument!\n"
		  << "Expected between 0 and 10, got " << newQuality;

		throw std::runtime_error { s.str() };
	}

	if (quality == newQuality)
		return;

	quality = newQuality;

	if (initialised)
		update_filter();
}

int Resampler::get_quality() const noexcept
{
	return quality;
}

void Resampler::set_input_stride (std::uint32_t stride) noexcept
{
	in_stride = stride;
}

std::uint32_t Resampler::get_input_stride() const noexcept
{
	return in_stride;
}

void Resampler::set_output_stride (std::uint32_t stride) noexcept
{
	out_stride = stride;
}

std::uint32_t Resampler::get_output_stride() const noexcept
{
	return out_stride;
}

int Resampler::get_input_latency() const noexcept
{
	return filt_len / 2;
}

int Resampler::get_output_latency() const noexcept
{
	return static_cast<int> ((static_cast<std::uint32_t> (get_input_latency()) * den_rate + (num_rate >> 1)) / num_rate);
}

void Resampler::skip_zeroes() noexcept
{
	const auto skipNum = get_input_latency();

	for (unsigned i = 0; i < nb_channels; ++i)
		last_sample[i] = skipNum;
}

void Resampler::reset_mem()
{
	for (unsigned i = 0; i < nb_channels * (filt_len - 1); ++i)
		mem[i] = 0.f;

	for (unsigned i = 0; i < nb_channels; ++i)
	{
		last_sample[i]	 = 0;
		magic_samples[i] = 0;
		samp_frac_num[i] = 0;
	}
}

std::uint32_t Resampler::get_num_channels() const noexcept
{
	return nb_channels;
}

}  // namespace speex

LIMES_END_NAMESPACE
