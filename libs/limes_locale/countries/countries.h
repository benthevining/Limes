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

/** @defgroup loc_country Countries
	Utilities for working with different countries.
	@ingroup limes_locale
 */

/** @file
	This file contains utilities for working with metadata about various countries.
	@ingroup loc_country
 */

LIMES_BEGIN_NAMESPACE

namespace locale
{

struct Language;

/** This struct represents metadata about a country.
	@ingroup loc_country
 */
struct LIMES_EXPORT Country final
{
	/** Returns a %vector of Language objects representing languages that may be spoken in this country. */
	[[nodiscard]] ds::vector<Language> getLanguages() const;

	/** Returns true if all the metadata fields contain valid data. */
	[[nodiscard]] bool isValid() const noexcept;

	/** The full name of this country. */
	std::string countryName;

	/** The ISO country code for this country. */
	std::string countryCode;

	/*
	- continent
	- web domain
	- phone code
	- drives on right
	- currency
	*/
};

/** Returns the Country object representing the given ISO country code.
	A null Country object may be returned if no known countries match the given code; you should call \c %isValid() on the returned %Country object before using it.
	@ingroup loc_country
 */
[[nodiscard]] Country getCountryByCode (const std::string_view& countryCode);

/** Returns the Country object representing the country with the given name.
	A null Country object may be returned if no known countries match the given code; you should call \c %isValid() on the returned %Country object before using it.
	@ingroup loc_country
 */
[[nodiscard]] Country getCountryByName (const std::string_view& countryName);

/** Returns a vector of all known Country objects.
	These should not be mutated!
	@ingroup loc_country
 */
[[nodiscard]] const ds::vector<Country>& getAllCountries();

}  // namespace locale

LIMES_END_NAMESPACE
