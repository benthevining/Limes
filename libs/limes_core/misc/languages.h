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

#include "../data_structures/vectors/vector.h"
#include <limes_export.h>

namespace lemons::locale
{

/** @defgroup lemons_locale Localization
	@ingroup lemons_core
	Utilities for internationalization.
 */


/** @defgroup lemons_iso_language_codes ISO language codes
	@ingroup lemons_locale
 */

/** @ingroup lemons_iso_language_codes
	Converts the name of a language to an ISO639-1 or ISO639-2 language code.
	@param name The name of the language. If the passed language name cannot be located in the list of known languages, an empty string will be returned.
	@param asISO639_2 When true, the returned language code will be a 3-character ISO639-2 code. When false, the returned language code will be a 2-character ISO639-1 code. Note that not all known languages have ISO639-1 codes, as some languages were added to the ISO639-2 extension of the standard.
	@see languageCodeToName()
 */
LIMES_EXPORT [[nodiscard]] std::string languageNameToCode (const std::string& name, bool asISO639_2 = true);


/** @ingroup lemons_iso_language_codes
	Converts an ISO language code to the name of the language it represents.
	If the passed string is 2 characters long, it will be interpreted as an ISO639-1 code; if it is 3 characters long, it will be interpreted as an ISO639-2 code. Other lengths of passed strings are invalid and will cause this method to return an empty string.
	If the language cannot be identified in the list of known languages, an empty string will be returned.
	@see languageNameToCode()
 */
LIMES_EXPORT [[nodiscard]] std::string languageCodeToName (const std::string& languageCode);


/** @defgroup lemons_iso_country_codes ISO country codes
	@ingroup lemons_locale
 */

/** @ingroup lemons_iso_country_codes
	Returns a list of 2-character ISO 3166 country codes where the specified language is an officially recognized language, a significant minority language, or a local indigenous language.
	These country codes are referenced from https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes.
	@attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage().
	@param language The language name or language code. When interpreting this argument as a language code, a 2-character string will be interpreted as an ISO639-1 code; if it is 3 characters long, it will be interpreted as an ISO639-2 code. Other lengths of passed strings are invalid and will cause this method to return an empty string.
	@param languageName If true, the 'language' argument is interpreted as the name of the language; if false, it is interpreted as an ISO language code.
 */
LIMES_EXPORT [[nodiscard]] StringVector getCountryCodesForLanguage (const std::string& language, bool languageName = true);


/** @ingroup lemons_iso_country_codes
	Returns a list of language names that are officially recognized, significant minority languages, or local indigenous languages for the specified country.
	@param countryCode A 2-character ISO 3166 country code. If an invalid country code is passed, this method will return an empty StringArray.
	@attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage().
	@see getLanguageCodesForCountry()
 */
LIMES_EXPORT [[nodiscard]] StringVector getLanguageNamesForCountry (const std::string& countryCode);


/** @ingroup lemons_iso_country_codes
	Returns a list of language codes for languages that are officially recognized, significant minority languages, or local indigenous languages for the specified country.
	@param countryCode A 2-character ISO 3166 country code. If an invalid country code is passed, this method will return an empty StringArray.
	@attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage().
	@see getLanguageNamesForCountry()
 */
LIMES_EXPORT [[nodiscard]] StringVector getLanguageCodesForCountry (const std::string& countryCode, bool asISO639_2 = true);


}  // namespace lemons::locale
