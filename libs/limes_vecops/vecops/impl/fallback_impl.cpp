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

#include <cmath>			 // for fabs, fabsf, sqrtf
#include <limes_platform.h>	 // for LIMES_FORCE_INLINE
#include <limes_namespace.h>
#include "./fallback_impl.h"
#include <type_traits>

LIMES_BEGIN_NAMESPACE

namespace vecops::fb::detail
{

template <Scalar DataType>
LIMES_FORCE_INLINE static DataType approximate_atan2 (DataType real, DataType imag) noexcept
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

	const auto cond = [z]() -> DataType
	{
		if constexpr (std::is_same_v<DataType, float>)
			return fabsf (z) < DataType (1.);  // NB. a GCC bug prevents fabsf from showing up in the std namespace
		else
			return std::fabs (z) < DataType (1.);
	}();

	if (cond > DataType (0.))
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
void magphase (T* const mag, T* const phase, T real, T imag) noexcept
{
	*phase = approximate_atan2 (real, imag);

	if constexpr (std::is_same_v<T, float>)
		*mag = sqrtf (real * real + imag * imag);
	else
		*mag = std::sqrt (real * real + imag * imag);
}

template void magphase (float* const, float* const, float, float);
template void magphase (double* const, double* const, double, double);


template <Scalar DataType>
LIMES_NO_EXPORT [[nodiscard]] LIMES_FORCE_INLINE DataType quake3_fast_inv_sqrt (DataType number) noexcept
{
	static_assert (std::is_same_v<DataType, double> || std::is_same_v<DataType, float>);

	using IntType = std::conditional_t<std::is_same_v<DataType, double>,
									   std::int64_t,
									   std::int32_t>;

	const auto x2 = number * DataType (0.5);

	auto i = *reinterpret_cast<IntType*> (&number);

	const auto magicConstant = []() -> IntType
	{
		if constexpr (std::is_same_v<DataType, double>)
			return 0x5fe6eb50c7b537a9;
		else
			return 0x5f3759df;
	}();

	i = magicConstant - (i >> IntType (1));

	const auto y = *reinterpret_cast<DataType*> (&i);

	return y * DataType (1.5) - (x2 * y * y);
}

template float	quake3_fast_inv_sqrt (float);
template double quake3_fast_inv_sqrt (double);

}  // namespace vecops::fb::detail

LIMES_END_NAMESPACE
