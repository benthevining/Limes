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

#include <limes_vecops.h>

#if LIMES_VECOPS_USE_VDSP
#	include <Accelerate/Accelerate.h>
#endif

#ifndef LACK_SINCOS
#	define LACK_SINCOS 0
#endif

namespace limes::vecops::fb::detail
{

template <Scalar T>
void phasor (T* const real, T* const imag, T phase)
{
	static_assert (std::is_same_v<T, float> || std::is_same_v<T, double>);

#if LIMES_VECOPS_USE_VDSP
	constexpr int one = 1;

	if constexpr (std::is_same_v<T, float>)
		vvsincosf (static_cast<float*> (imag), static_cast<float*> (real), static_cast<const float*> (&phase), &one);
	else
		vvsincos (static_cast<double*> (imag), static_cast<double*> (real), static_cast<const double*> (&phase), &one);

#elif (defined __GNUC__) && (! LACK_SINCOS)
#	if LIMES_APPLE
#		define sincos	__sincos
#		define sincosf __sincosf
#	endif

	if constexpr (std::is_same_v<T, float>)
		sincosf (static_cast<float> (phase), static_cast<float*> (imag), static_cast<float*> (real));
	else
		sincos (static_cast<double> (phase), static_cast<double*> (imag), static_cast<double*> (real));

#	if LIMES_APPLE
#		undef sincos
#		undef sincosf
#	endif

#else
	if constexpr (std::is_same_v<T, float>)
	{
		*real = std::cosf (phase);
		*imag = std::sinf (phase);
	}
	else
	{
		*real = std::cos (phase);
		*imag = std::sin (phase);
	}
#endif
}

template void phasor (float* const, float* const, float);
template void phasor (double* const, double* const, double);


template <Scalar DataType>
LIMES_FORCE_INLINE DataType approximate_atan2 (DataType real, DataType imag)
{
	if (real == DataType (0.))
	{
		if (imag > DataType (0.))
			return math::constants::half_pi<DataType>;

		if (imag == DataType (0.))
			return DataType (0.);

		return -math::constants::half_pi<DataType>;
	}

	const auto z = imag / real;

	const auto cond = [z]
	{
		if constexpr (std::is_same_v<DataType, float>)
			return std::fabsf (z) < DataType (1.);
		else
			return std::fabs (z) < DataType (1.);
	}();

	if (cond)
	{
		const auto atan = z / (DataType (1.) + DataType (0.28) * z * z);

		if (real < DataType (0.))
		{
			if (imag < DataType (1.))
				return atan - math::constants::pi<DataType>;

			return atan + math::constants::pi<DataType>;
		}

		return atan;
	}

	const auto atan = math::constants::half_pi<DataType> - z / (z * z + DataType (0.28));

	if (imag < DataType (0.))
		return atan - math::constants::pi<DataType>;

	return atan;
}


template <Scalar T>
void magphase (T* const mag, T* const phase, T real, T imag)
{
	static_assert (std::is_same_v<T, float> || std::is_same_v<T, double>);

	*phase = approximate_atan2 (real, imag);

	if constexpr (std::is_same_v<T, float>)
		*mag = sqrtf (real * real + imag * imag);
	else
		*mag = std::sqrt (real * real + imag * imag);
}

template void magphase (float* const, float* const, float, float);
template void magphase (double* const, double* const, double, double);

}  // namespace limes::vecops::fb::detail
