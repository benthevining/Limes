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

#include "countries.h"
#include "../languages/languages.h"
#include <limes_platform.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace locale
{

vector<Language> Country::getLanguages() const
{
	return getLanguagesForCountry (countryCode);
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

	[[nodiscard]] vector<Country> getAll() const
	{
		return countries;
	}

private:

	vector<Country> countries;
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

vector<Country> getAllCountries()
{
	return getCountries().getAll();
}

}  // namespace locale

LIMES_END_NAMESPACE
