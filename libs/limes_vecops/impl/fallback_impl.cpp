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

#include <limes_vecops.h>	 // for magphase
#include <cmath>			 // for fabs, fabsf, sqrtf
#include "limes_platform.h"	 // for LIMES_FORCE_INLINE

namespace limes::vecops::fb::detail
{

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
			return fabsf (z) < DataType (1.);  // NB. a GCC bug prevents fabf from showing up in the std namespace
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
