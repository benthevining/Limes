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

#include "Coeffecients.h"
#include <limes_platform.h>
#include <limes_core.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace dsp::filters
{

template <Sample Sampletype>
Coeffecients<Sampletype>::Storage::Storage()
{
	coefficients.reserveAndZero (8);
}

template <Sample Sampletype>
scalar_vector<Sampletype>* Coeffecients<Sampletype>::Storage::operator->() noexcept
{
	return &coefficients;
}

template <Sample Sampletype>
const scalar_vector<Sampletype>* Coeffecients<Sampletype>::Storage::operator->() const noexcept
{
	return &coefficients;
}

template <Sample Sampletype>
typename Coeffecients<Sampletype>::Storage& Coeffecients<Sampletype>::Storage::operator= (std::initializer_list<Sampletype> list)
{
	coefficients.clear();

	const auto a0Index = [size = list.size()]
	{
		switch (size)
		{
			case (6) :
				return 3;
			case (8) :
				return 4;
			default :
				return 2;
		}
	}();

	int index = 0;

	Sampletype a0inv = 0;

	for (auto& element : list)
	{
		if (index == a0Index)
			a0inv = static_cast<Sampletype> (Sampletype (1.) / element);
		else
			coefficients.emplace_back (element);

		++index;
	}

	coefficients.multiply (a0inv);

	return *this;
}

/*--------------------------------------------------------------------------------------------------------------*/

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeFirstOrderLowPass (double	 sampleRate,
													  Sampletype frequency)
{
	LIMES_ASSERT (sampleRate > 0.0);
	LIMES_ASSERT (frequency > 0 && frequency <= static_cast<float> (sampleRate * 0.5));

	const auto n = std::tan (math::constants::pi<Sampletype> * frequency
							 / static_cast<Sampletype> (sampleRate));

	coefficients = { n, n, n + 1, n - 1 };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeFirstOrderHighPass (double	  sampleRate,
													   Sampletype frequency)
{
	LIMES_ASSERT (sampleRate > 0.0);
	LIMES_ASSERT (frequency > 0 && frequency <= static_cast<float> (sampleRate * 0.5));

	const auto n = std::tan (math::constants::pi<Sampletype> * frequency
							 / static_cast<Sampletype> (sampleRate));

	coefficients = { 1, -1, n + 1, n - 1 };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeFirstOrderAllPass (double	 sampleRate,
													  Sampletype frequency)
{
	LIMES_ASSERT (sampleRate > 0.0);
	LIMES_ASSERT (frequency > 0 && frequency <= static_cast<float> (sampleRate * 0.5));

	const auto n = std::tan (math::constants::pi<Sampletype> * frequency
							 / static_cast<Sampletype> (sampleRate));

	coefficients = { n - 1, n + 1, n + 1, n - 1 };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeLowPass (double	   sampleRate,
											Sampletype frequency,
											Sampletype Q)
{
	const auto n = 1
				 / std::tan (math::constants::pi<Sampletype> * frequency
							 / static_cast<Sampletype> (sampleRate));
	const auto nSquared = n * n;
	const auto invQ		= (Sampletype) 1 / Q;
	const auto c1		= 1 / (1 + invQ * n + nSquared);

	coefficients = {
		c1, c1 * 2, c1, 1, c1 * 2 * (1 - nSquared), c1 * (1 - invQ * n + nSquared)
	};
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeHighPass (double		sampleRate,
											 Sampletype frequency,
											 Sampletype Q)
{
	const auto n		= std::tan (math::constants::pi<Sampletype> * frequency
									/ static_cast<Sampletype> (sampleRate));
	const auto nSquared = n * n;
	const auto invQ		= (Sampletype) 1 / Q;
	const auto c1		= 1 / (1 + invQ * n + nSquared);

	coefficients = {
		c1, c1 * -2, c1, 1, c1 * 2 * (nSquared - 1), c1 * (1 - invQ * n + nSquared)
	};
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeBandPass (double		sampleRate,
											 Sampletype frequency,
											 Sampletype Q)
{
	LIMES_ASSERT (sampleRate > 0.0);
	LIMES_ASSERT (frequency > 0 && frequency <= static_cast<float> (sampleRate * 0.5));
	LIMES_ASSERT (Q > 0.0);

	const auto n = 1
				 / std::tan (math::constants::pi<Sampletype> * frequency
							 / static_cast<Sampletype> (sampleRate));
	const auto nSquared = n * n;
	const auto invQ		= 1 / Q;
	const auto c1		= 1 / (1 + invQ * n + nSquared);

	coefficients = { c1 * n * invQ,
					 0,
					 -c1 * n * invQ,
					 1,
					 c1 * 2 * (1 - nSquared),
					 c1 * (1 - invQ * n + nSquared) };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeNotch (double	 sampleRate,
										  Sampletype frequency,
										  Sampletype Q)
{
	LIMES_ASSERT (sampleRate > 0.0);
	LIMES_ASSERT (frequency > 0 && frequency <= static_cast<float> (sampleRate * 0.5));
	LIMES_ASSERT (Q > 0.0);

	const auto n = 1
				 / std::tan (math::constants::pi<Sampletype> * frequency
							 / static_cast<Sampletype> (sampleRate));

	const auto nSquared = n * n;
	const auto invQ		= 1 / Q;
	const auto c1		= 1 / (1 + n * invQ + nSquared);
	const auto b0		= c1 * (1 + nSquared);
	const auto b1		= 2 * c1 * (1 - nSquared);

	coefficients = { b0, b1, b0, 1, b1, c1 * (1 - n * invQ + nSquared) };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeAllPass (double	   sampleRate,
											Sampletype frequency,
											Sampletype Q)
{
	LIMES_ASSERT (sampleRate > 0);
	LIMES_ASSERT (frequency > 0 && frequency <= sampleRate * 0.5);
	LIMES_ASSERT (Q > 0);

	const auto n = 1
				 / std::tan (math::constants::pi<Sampletype> * frequency
							 / static_cast<Sampletype> (sampleRate));

	const auto nSquared = n * n;
	const auto invQ		= 1 / Q;
	const auto c1		= 1 / (1 + invQ * n + nSquared);
	const auto b0		= c1 * (1 - n * invQ + nSquared);
	const auto b1		= c1 * 2 * (1 - nSquared);

	coefficients = { b0, b1, 1, 1, b1, b0 };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeLowShelf (double		sampleRate,
											 Sampletype cutOffFrequency,
											 Sampletype Q,
											 Sampletype gainFactor)
{
	LIMES_ASSERT (sampleRate > 0.0);
	LIMES_ASSERT (cutOffFrequency > 0.0 && cutOffFrequency <= sampleRate * 0.5);
	LIMES_ASSERT (Q > 0.0);

	const auto A	   = std::max (static_cast<Sampletype> (0.0), std::sqrt (gainFactor));
	const auto aminus1 = A - 1;
	const auto aplus1  = A + 1;
	const auto omega   = (2 * math::constants::pi<Sampletype> * std::max (cutOffFrequency, static_cast<Sampletype> (2.0)))
					 / static_cast<Sampletype> (sampleRate);

	const auto coso				= std::cos (omega);
	const auto beta				= std::sin (omega) * std::sqrt (A) / Q;
	const auto aminus1TimesCoso = aminus1 * coso;

	coefficients = { A * (aplus1 - aminus1TimesCoso + beta),
					 A * 2 * (aminus1 - aplus1 * coso),
					 A * (aplus1 - aminus1TimesCoso - beta),
					 aplus1 + aminus1TimesCoso + beta,
					 -2 * (aminus1 + aplus1 * coso),
					 aplus1 + aminus1TimesCoso - beta };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makeHighShelf (double	 sampleRate,
											  Sampletype cutOffFrequency,
											  Sampletype Q,
											  Sampletype gainFactor)
{
	LIMES_ASSERT (sampleRate > 0);
	LIMES_ASSERT (cutOffFrequency > 0
				  && cutOffFrequency <= static_cast<Sampletype> (sampleRate * 0.5));
	LIMES_ASSERT (Q > 0);

	const auto A	   = std::max (static_cast<Sampletype> (0.0), std::sqrt (gainFactor));
	const auto aminus1 = A - 1;
	const auto aplus1  = A + 1;
	const auto omega   = (2 * math::constants::pi<Sampletype> * std::max (cutOffFrequency, static_cast<Sampletype> (2.0)))
					 / static_cast<Sampletype> (sampleRate);

	const auto coso				= std::cos (omega);
	const auto beta				= std::sin (omega) * std::sqrt (A) / Q;
	const auto aminus1TimesCoso = aminus1 * coso;

	coefficients = { A * (aplus1 + aminus1TimesCoso + beta),
					 A * -2 * (aminus1 + aplus1 * coso),
					 A * (aplus1 + aminus1TimesCoso - beta),
					 aplus1 - aminus1TimesCoso + beta,
					 2 * (aminus1 - aplus1 * coso),
					 aplus1 - aminus1TimesCoso - beta };
}

template <Sample Sampletype>
void Coeffecients<Sampletype>::makePeakFilter (double	  sampleRate,
											   Sampletype frequency,
											   Sampletype Q,
											   Sampletype gainFactor)
{
	LIMES_ASSERT (sampleRate > 0);
	LIMES_ASSERT (frequency > 0
				  && frequency <= static_cast<Sampletype> (sampleRate * 0.5));
	LIMES_ASSERT (Q > 0);
	LIMES_ASSERT (gainFactor > 0);

	const auto A	 = std::max (static_cast<Sampletype> (0.0), std::sqrt (gainFactor));
	const auto omega = (2 * math::constants::pi<Sampletype> * std::max (frequency, static_cast<Sampletype> (2.0)))
					 / static_cast<Sampletype> (sampleRate);

	const auto alpha	   = std::sin (omega) / (Q * 2);
	const auto c2		   = -2 * std::cos (omega);
	const auto alphaTimesA = alpha * A;
	const auto alphaOverA  = alpha / A;

	coefficients = {
		1 + alphaTimesA, c2, 1 - alphaTimesA, 1 + alphaOverA, c2, 1 - alphaOverA
	};
}

template <Sample Sampletype>
int Coeffecients<Sampletype>::getFilterOrder() const noexcept
{
	return (coefficients->numObjects() - 1) / 2;
}

template <Sample Sampletype>
Sampletype* Coeffecients<Sampletype>::getRawCoefficients() noexcept
{
	return coefficients->data();
}

template <Sample Sampletype>
const Sampletype* Coeffecients<Sampletype>::getRawCoefficients() const noexcept
{
	return coefficients->data();
}

template class Coeffecients<float>;
template class Coeffecients<double>;

}  // namespace dsp::filters

LIMES_END_NAMESPACE
