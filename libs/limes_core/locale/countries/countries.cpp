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

#include "./countries.h"
#include "../languages/languages.h"
#include "../../limes_namespace.h"
#include <limes_platform.h>
#include "../../system/limes_assert.h"
#include "../../misc/Algorithm.h"

LIMES_BEGIN_NAMESPACE

namespace locale
{

std::vector<Language> Country::getLanguages() const
{
	return getLanguagesForCountry (countryCode);
}

bool Country::isValid() const noexcept
{
	return (! countryName.empty()) && (! countryCode.empty());
}


struct KnownCountries final
{
	KnownCountries()
	{
	}

	[[nodiscard]] Country getCountryForCode (const std::string& countryCode) const
	{
		LIMES_ASSERT (! countryCode.empty());

		return alg::contains_or_default (countries,
										 [&countryCode] (const Country& country)
										 { return country.countryCode == countryCode; });
	}

	[[nodiscard]] Country getCountryByName (const std::string& countryName) const
	{
		LIMES_ASSERT (! countryName.empty());

		return alg::contains_or_default (countries,
										 [&countryName] (const Country& country)
										 { return country.countryName == countryName; });
	}

	[[nodiscard]] const std::vector<Country>& getAll() const
	{
		return countries;
	}

private:
	std::vector<Country> countries;
};


static const KnownCountries known_countries;

static const KnownCountries& getCountries()
{
	return known_countries;
}


Country getCountryByCode (const std::string_view& countryCode)
{
	return getCountries().getCountryForCode (std::string { countryCode });
}

Country getCountryByName (const std::string_view& countryName)
{
	return getCountries().getCountryByName (std::string { countryName });
}

const std::vector<Country>& getAllCountries()
{
	return getCountries().getAll();
}

}  // namespace locale

LIMES_END_NAMESPACE
