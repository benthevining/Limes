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

#include <limes_export.h>
#include <limes_data_structures.h>
#include <string>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace locale
{

struct Language;


struct LIMES_EXPORT Country final
{
	[[nodiscard]] ds::vector<Language> getLanguages() const;

	std::string countryCode, countryName;

	/*
	- continent
	- web domain
	- phone code
	- drives on right
	- currency
	*/
};


[[nodiscard]] Country getCountryByCode (const std::string_view& countryCode);

[[nodiscard]] Country getCountryByName (const std::string_view& countryName);

[[nodiscard]] ds::vector<Country> getAllCountries();

}  // namespace locale

LIMES_END_NAMESPACE
