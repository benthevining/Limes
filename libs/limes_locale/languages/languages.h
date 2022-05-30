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

/** @defgroup loc_lang Languages
	Utilities for working with spoken human languages.
	@ingroup limes_locale
 */

/** @dir libs/limes_locale/languages
	This directory contains utilities for working with spoken human languages.
	@ingroup loc_lang
 */

/** @file
	This file contains utilities for retrieving information about languages.
	@ingroup loc_lang
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for localization and internationalization.
	@ingroup loc_lang
 */
namespace locale
{

struct Country;

/** This struct represents metadata about a spoken human %language.
	@see getLanguageForCode(), getLanguage(), getLanguagesForCountry(), getAllKnownLanguages()
	@ingroup loc_lang
 */
struct LIMES_EXPORT Language final
{
	/** @name Constructors */
	///@{
	/** This library contains an internal database of known languages; you should rarely need to construct one of these objects yourself. */
	explicit Language (const std::string_view& name,
					   const std::string_view& ISO_639_1, const std::string_view& ISO_639_2,
					   const ds::StringVector& countries   = {},  // NOLINT
					   bool					   rightToLeft = false);

	/** Constructs a null %language object containing no metadata. */
	Language() = default;
	///@}

	/** Returns true if all the metadata fields contain valid data. */
	[[nodiscard]] bool isValid() const noexcept;

	/** Returns true if this %language is likely to be spoken by a meaningful proportion of the given country's population. */
	[[nodiscard]] bool isForCountry (const std::string_view& countryCode) const;

	/** Returns a %vector of Country objects representing countries in which a meaningful proportion of the population may speak this %language. */
	[[nodiscard]] ds::vector<Country> getCountries() const;

	/** The full name of the %language. */
	std::string languageName;

	/** The 2-digit ISO 639-1 code identifying this %language.
		@note Not every %language will have an ISO 639-1 code.
		They are mostly provided for compatibility; you should prefer to reference ISO 639-2 codes.
	 */
	std::string ISO639_1;

	/** The 3-digit ISO 639-2 code identifying this %language.
		Every known %language will have one of these.
	 */
	std::string ISO639_2;

	/** A vector of ISO country codes identifying countries where a meaningful proportion of the population may speak this %language. */
	ds::StringVector countryCodes;

	/** True if this %language is written right to left. */
	bool isRightToLeft { false };
};

/** Returns the Language object representing the given ISO %language code.
	If the given %language code is 2 characters, it is interpreted as an ISO 639-1 code. If it is 3 characters, it's interpreted as an ISO 639-2 code. Otherwise, an assertion is thrown.
	A null Language object may be returned if no known %language matches the given ISO %language code; you should check \c %isValid() on the returned %Language object before using it.
	@ingroup loc_lang
	@see getLanguage()
 */
[[nodiscard]] LIMES_EXPORT Language getLanguageForCode (const std::string_view& ISOlanguageCode);

/** Returns the Language object representing the %language with the given name.
	If the passed %language name is empty, an assertion is thrown.
	A null Language object may be returned if no known language matches the given %language name; you should check \c %isValid() on the returned %Language object before using it.
	@ingroup loc_lang
	@see getLanguageForCode()
 */
[[nodiscard]] LIMES_EXPORT Language getLanguage (const std::string_view& languageName);

/** Returns a %vector of Language objects representing languages that may be spoken in the given %country.
	@ingroup loc_lang
 */
[[nodiscard]] LIMES_EXPORT ds::vector<Language> getLanguagesForCountry (const std::string_view& countryCode);

/** Returns all known Language objects in Limes's internal database.
	These should not be mutated!
	@ingroup loc_lang
 */
[[nodiscard]] LIMES_EXPORT const ds::vector<Language>& getAllKnownLanguages();

}  // namespace locale

LIMES_END_NAMESPACE
