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

#include <limes_export.h>			// for LIMES_EXPORT
#include <string>					// for string
#include <limes_data_structures.h>	// for StringVector
#include <limes_platform.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace locale
{

struct Country;


struct LIMES_EXPORT Language final
{
	explicit Language (const std::string_view& name,
					   const std::string_view& ISO_639_1, const std::string_view& ISO_639_2,
					   const StringVector& countries   = {},  // NOLINT
					   bool				   rightToLeft = false);

	Language() = default;

	[[nodiscard]] bool isForCountry (const std::string_view& countryCode) const;

	[[nodiscard]] vector<Country> getCountries() const;

	std::string languageName, ISO639_1, ISO639_2;

	StringVector countryCodes;

	bool isRightToLeft { false };
};


[[nodiscard]] LIMES_EXPORT Language getLanguageForCode (const std::string_view& ISOlanguageCode);

[[nodiscard]] LIMES_EXPORT Language getLanguage (const std::string_view& languageName);

[[nodiscard]] LIMES_EXPORT vector<Language> getLanguagesForCountry (const std::string_view& countryCode);

[[nodiscard]] LIMES_EXPORT vector<Language> getAllKnownLanguages();

}  // namespace locale

LIMES_END_NAMESPACE
