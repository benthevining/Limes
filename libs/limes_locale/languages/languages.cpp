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

#include "languages.h"
#include <string>					// for operator==, basic_string, operator!=
#include <string_view>				// for string_view
#include <limes_platform.h>			// for LIMES_ASSERT
#include <limes_core.h>				// for contains_or_default, contains
#include <limes_data_structures.h>	// for vector, SV, basic_vector
#include "../countries/countries.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace locale
{

Language::Language (const std::string_view& name,
					const std::string_view& ISO_639_1, const std::string_view& ISO_639_2,
					const ds::StringVector& countries,
					bool					rightToLeft)
	: languageName (name), ISO639_1 (ISO_639_1), ISO639_2 (ISO_639_2), countryCodes (countries), isRightToLeft (rightToLeft)
{
	LIMES_ASSERT (ISO639_1.empty() || ISO639_1.length() == 2);
	LIMES_ASSERT (ISO639_2.length() == 3);
}

bool Language::isForCountry (const std::string_view& countryCode) const
{
	return alg::contains (countryCodes, std::string { countryCode });
}

ds::vector<Country> Language::getCountries() const
{
	ds::vector<Country> countries;

	for (const auto& code : countryCodes)
		countries.emplace_back (getCountryByCode (code));  // cppcheck-suppress useStlAlgorithm

	return countries;
}

bool Language::isValid() const noexcept
{
	return (! languageName.empty()) && ((! ISO639_1.empty()) || (! ISO639_2.empty()));
}


/** Note that ISO639-2/B codes are used instead of ISO639-2/T codes.
	See https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
	And https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
 */
struct KnownLanguages final
{
	KnownLanguages()
	{
		using SV = ds::StringVector;

		languages.emplace_back ("Abkhazian", "ab", "abk", SV { "GE" });
		languages.emplace_back ("Achinese", "", "ace", SV { "ID" });
		languages.emplace_back ("Acoli", "", "ach", SV { "UG" });
		languages.emplace_back ("Adangme", "", "ada", SV { "GH" });
		languages.emplace_back ("Adyghe", "", "ady", SV { "RU" });
		languages.emplace_back ("Afar", "aa", "aar", SV { "DJ", "ER", "ET" });
		languages.emplace_back ("Afrikaans", "af", "afr", SV { "NA", "ZA" });
		languages.emplace_back ("Ainu", "", "ain", SV { "RU", "JP" });
		languages.emplace_back ("Akan", "ak", "aka", SV { "CI", "GH" });
		languages.emplace_back ("Albanian", "sq", "alb", SV { "AL", "IT", "MK", "ME", "RO" });
		languages.emplace_back ("Aleut", "", "ale", SV { "US", "RU" });
		languages.emplace_back ("Amharic", "am", "amh", SV { "ET" });
		languages.emplace_back ("Angika", "", "anp", SV { "IN", "NP" });
		languages.emplace_back ("Arabic", "ar", "ara",
								SV { "DZ", "BH", "BJ", "TD", "KM", "DJ", "EG", "ER", "GM", "GN", "GW", "ID", "IR", "IL", "JO", "KW", "LB", "LY", "ML", "MR", "MA", "NE", "OM", "PS", "PH", "QA", "SA", "SN", "SO", "ZA", "SD", "SY", "TJ", "TZ", "TN", "AE", "UZ", "VE", "EH", "YE" },
								true);
		languages.emplace_back ("Aragonese", "an", "arg", SV { "ES" });
		languages.emplace_back ("Arapaho", "", "arp", SV { "US" });
		languages.emplace_back ("Arawak", "", "arw", SV { "SR", "VE", "GY" });
		languages.emplace_back ("Armenian", "hy", "arm", SV { "AM", "CY", "IQ", "RO" });
		languages.emplace_back ("Aromanian", "", "rup", SV { "MK" });
		languages.emplace_back ("Assamese", "as", "asm", SV { "IN" });
		languages.emplace_back ("Avaric", "av", "ava", SV { "RU" });
		languages.emplace_back ("Awadhi", "", "awa", SV { "NP" });
		languages.emplace_back ("Aymara", "ay", "aym", SV { "BO", "PE" });
		languages.emplace_back ("Azerbaijani", "az", "aze", SV { "AZ", "IR", "KZ", "TM", "UZ" });
		languages.emplace_back ("Balinese", "", "ban", SV { "ID" });
		languages.emplace_back ("Baluchi", "", "bal", SV { "PK", "TM", "IR" });
		languages.emplace_back ("Bambara", "bm", "bam", SV { "ML" });
		languages.emplace_back ("Basa", "", "bas", SV { "CM" });
		languages.emplace_back ("Bashkir", "ba", "bak", SV { "RU" });
		languages.emplace_back ("Basque", "eu", "baq", SV { "ES" });
		languages.emplace_back ("Beja", "", "bej", SV { "ER" });
		languages.emplace_back ("Belarusian", "be", "bel", SV { "BY", "CZ", "UA" });
		languages.emplace_back ("Bemba", "", "bem", SV { "ZM" });
		languages.emplace_back ("Bengali", "bn", "ben", SV { "BD", "IN", "ID" });
		languages.emplace_back ("Bhojpuri", "", "bho", SV { "NP" });
		languages.emplace_back ("Bikol", "", "bik", SV { "PH" });
		languages.emplace_back ("Bini", "", "bin", SV { "NG" });
		languages.emplace_back ("Bislama", "bi", "bis", SV { "VU" });
		languages.emplace_back ("Bilin", "", "byn", SV { "ER" });
		languages.emplace_back ("Bokmål", "", "nob", SV { "NO" });
		languages.emplace_back ("Bosnian", "bs", "bos", SV { "BA", "MK", "ME" });
		languages.emplace_back ("Braj", "", "bra", SV { "IN" });
		languages.emplace_back ("Breton", "br", "bre", SV { "FR" });
		languages.emplace_back ("Buginese", "", "bug", SV { "ID" });
		languages.emplace_back ("Bulgarian", "bg", "bul", SV { "BG", "RO", "UA" });
		languages.emplace_back ("Buriat", "", "bua", SV { "RU" });
		languages.emplace_back ("Burmese", "my", "bur", SV { "MM" });
		languages.emplace_back ("Caddo", "", "cad", SV { "US" });
		languages.emplace_back ("Catalan", "ca", "cat", SV { "AD", "IT", "ES" });
		languages.emplace_back ("Cebuano", "", "ceb", SV { "PH" });
		languages.emplace_back ("Central Khmer", "", "khm", SV { "KH" });
		languages.emplace_back ("Chamorro", "ch", "cha", SV { "GU", "MP" });
		languages.emplace_back ("Chechen", "ce", "che", SV { "RU" });
		languages.emplace_back ("Cherokee", "", "chr", SV { "US" });
		languages.emplace_back ("Cheyenne", "", "chy", SV { "US" });
		languages.emplace_back ("Chibcha", "", "chb", SV { "CO" });
		languages.emplace_back ("Chichewa", "ny", "nya", SV { "MW", "MZ", "ZM", "ZW" });
		languages.emplace_back ("Chinese", "zh", "chi", SV { "BZ", "BN", "CN", "GY", "HK", "MO", "SR", "TW", "VE" });
		languages.emplace_back ("Chinook jargon", "", "chn", SV { "US" });
		languages.emplace_back ("Chipewyan", "", "chp", SV { "CA" });
		languages.emplace_back ("Choctaw", "", "cho", SV { "US" });
		languages.emplace_back ("Chuukese", "", "chk", SV { "FM" });
		languages.emplace_back ("Chuvash", "cv", "chv", SV { "RU" });
		languages.emplace_back ("Cornish", "kw", "cor", SV { "GB" });
		languages.emplace_back ("Corsican", "co", "cos", SV { "FR", "IT" });
		languages.emplace_back ("Cree", "cr", "cre", SV { "CA", "US" });
		languages.emplace_back ("Creek", "", "mus", SV { "US" });
		languages.emplace_back ("Crimean Tatar", "", "crh", SV { "RO", "RU", "UA" });
		languages.emplace_back ("Croatian", "hr", "hrv", SV { "AT", "BA", "HR", "ME", "RO" });
		languages.emplace_back ("Czech", "cs", "cze", SV { "CZ", "RO" });
		languages.emplace_back ("Dakota", "", "dak", SV { "CA", "US" });
		languages.emplace_back ("Danish", "da", "dan", SV { "DK", "FO" });
		languages.emplace_back ("Dargwa", "", "dar", SV { "RU" });
		languages.emplace_back ("Delaware", "", "del", SV { "CA", "US" });
		languages.emplace_back ("Dinka", "", "din", SV { "SS" });
		languages.emplace_back ("Divehi", "dv", "div", SV { "MV" }, true);
		languages.emplace_back ("Dogri", "", "doi", SV { "IN" });
		languages.emplace_back ("Dogrib", "", "dgr", SV { "CA" });
		languages.emplace_back ("Duala", "", "dua", SV { "CM" });
		languages.emplace_back ("Dutch", "nl", "dut", SV { "AW", "BE", "BQ", "CW", "GY", "ID", "NL", "SX", "SR" });
		languages.emplace_back ("Dyula", "", "dyu", SV { "GN", "GM", "CI" });
		languages.emplace_back ("Dzongkha", "dz", "dzo", SV { "BT" });
		languages.emplace_back ("Efik", "", "efi", SV { "NG" });
		languages.emplace_back ("Ekajuk", "", "eka", SV { "NG" });
		languages.emplace_back ("English", "en", "eng",
								SV { "AS", "AI", "AG", "AU", "AZ", "BS", "BH", "BB", "BZ", "BJ", "BM", "BQ", "BW", "IO", "BN", "BI", "CM", "CA", "KY", "CX", "CC", "CO", "CK", "CU", "CW", "DM", "ER", "SZ", "FK", "FJ", "FI", "GM", "GH", "GI", "GD", "GU", "GG", "GW", "GN", "GY",
									 "HK", "IN", "ID", "IQ", "IE", "JM", "KE", "KI", "LS", "LR", "MW", "MY", "MV", "MT", "MH", "MU", "FM", "NA", "NR", "NL", "NZ", "NI", "NG", "NU", "NF", "MP", "PK", "PW", "PG", "PH", "PN", "PR", "QA", "RU", "RW", "KN", "LC", "VC", "WS", "SC", "SL", "SG", "SX", "SB", "SO", "ZA", "GS", "SS", "LK", "SD", "SR", "TZ", "TH", "TL", "TK", "TO", "TT", "TC", "TV", "UG", "GB", "US", "VU", "VE", "VG", "VI", "ZM", "ZW" });
		languages.emplace_back ("Erzya", "", "myv", SV { "RU" });
		languages.emplace_back ("Esperanto", "eo", "epo");
		languages.emplace_back ("Estonian", "et", "est", SV { "EE" });
		languages.emplace_back ("Ewe", "ee", "ewe", SV { "GH", "TG" });
		languages.emplace_back ("Ewondo", "", "ewo", SV { "CM" });
		languages.emplace_back ("Fang", "", "fan", SV { "GA", "GQ" });
		languages.emplace_back ("Fanti", "", "fat", SV { "GH" });
		languages.emplace_back ("Faroese", "fo", "fao", SV { "FO" });
		languages.emplace_back ("Fijian", "fj", "fij", SV { "FJ" });
		languages.emplace_back ("Filipino", "", "fil", SV { "PH" });
		languages.emplace_back ("Finnish", "fi", "fin", SV { "FI", "SE" });
		languages.emplace_back ("Fon", "", "fon", SV { "BJ" });
		languages.emplace_back ("French", "fr", "fre",
								SV { "DZ", "BE", "BJ", "BF", "BI", "CM", "CA", "CF", "TD", "KM", "CD", "CG", "CI", "DJ", "GQ", "FR", "PF", "GM", "GN", "GY", "HT", "VA", "LA", "LB", "LU", "MG", "ML", "MR", "MU", "MC", "MA", "NC", "NE", "RU", "RW", "BL", "MF", "SN", "SC", "SR", "CH", "TG", "TN", "VU", "WF", "EH" });
		languages.emplace_back ("Friulian", "", "fur", SV { "IT" });
		languages.emplace_back ("Fulah", "ff", "ful", SV { "BJ", "CM", "GM", "GN", "ML", "NE", "NG" }, true);
		languages.emplace_back ("Ga", "", "gaa", SV { "GH" });
		languages.emplace_back ("Carib", "", "car", SV { "SR", "VE", "GY" });
		languages.emplace_back ("Galician", "gl", "glg", SV { "ES" });
		languages.emplace_back ("Ganda", "", "lug", SV { "UG" });
		languages.emplace_back ("Gayo", "", "gay", SV { "ID" });
		languages.emplace_back ("Gbaya", "", "gba", SV { "CD", "CF", "CM", "CG" });
		languages.emplace_back ("Georgian", "ka", "geo", SV { "GE" });
		languages.emplace_back ("German", "de", "ger",
								SV { "AT", "BE", "BZ", "CR", "CZ", "DK", "DE", "LI", "LU", "NA", "RO", "RU", "ZA", "CH", "VE" });
		languages.emplace_back ("Gilbertese", "", "gil", SV { "KI" });
		languages.emplace_back ("Gondi", "", "gon", SV { "IN" });
		languages.emplace_back ("Gorontalo", "", "gor", SV { "ID" });
		languages.emplace_back ("Grebo", "", "grb", SV { "LR" });
		languages.emplace_back ("Greek", "el", "gre", SV { "AL", "CY", "CZ", "GR", "RO", "ZA", "UA" });
		languages.emplace_back ("Guarani", "gn", "grn", SV { "AR", "BO", "PY" });
		languages.emplace_back ("Gujarati", "gu", "guj", SV { "IN", "ID", "ZA" });
		languages.emplace_back ("Gwich'in", "", "gwi", SV { "CA", "US" });
		languages.emplace_back ("Haida", "", "hai", SV { "CA" });
		languages.emplace_back ("Haitian", "ht", "hat", SV { "CU", "HT" });
		languages.emplace_back ("Hausa", "ha", "hau", SV { "BJ", "NE", "NG" });
		languages.emplace_back ("Hawaiian", "", "haw", SV { "US" });
		languages.emplace_back ("Hebrew", "he", "heb", SV { "IL", "ZA", "UA" }, true);
		languages.emplace_back ("Herero", "hz", "her", SV { "NA" });
		languages.emplace_back ("Hiligaynon", "", "hil", SV { "PH" });
		languages.emplace_back ("Hindi", "hi", "hin", SV { "IN", "ZA" });
		languages.emplace_back ("Hiri Motu", "ho", "hmo", SV { "PG" });
		languages.emplace_back ("Hmong", "", "hmn", SV { "LA" });
		languages.emplace_back ("Hungarian", "hu", "hun", SV { "AT", "CZ", "HU", "RO", "SI", "UA" });
		languages.emplace_back ("Hupa", "", "hup");
		languages.emplace_back ("Iban", "", "iba");
		languages.emplace_back ("Iloko", "", "ilo", SV { "PH" });
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Ingush", "", "inh", SV { "RU" });
		languages.emplace_back ("Interlingua", "ia", "ina");
		languages.emplace_back ("Indonesian", "id", "ind", SV { "ID", "TL" });
		languages.emplace_back ("Interlingue", "ie", "ile");
		languages.emplace_back ("Irish", "ga", "gle", SV { "IE", "GB" });
		languages.emplace_back ("Igbo", "ig", "ibo", SV { "NG" });
		languages.emplace_back ("Inupiaq", "ik", "ipk");
		languages.emplace_back ("Ido", "io", "ido");
		languages.emplace_back ("Icelandic", "is", "ice", SV { "IS" });
		languages.emplace_back ("Italian", "it", "ita", SV { "CR", "VA", "IT", "MT", "MC", "RO", "SM", "SI", "SO", "CH", "VE" });
		languages.emplace_back ("Inuktitut", "iu", "iku");
		languages.emplace_back ("Inupiaq", "", "ipk");
		languages.emplace_back ("Japanese", "ja", "jpn", SV { "ID", "JP", "PW" });
		languages.emplace_back ("Javanese", "jv", "jav", SV { "NC", "SR" });
		languages.emplace_back ("Judeo-Arabic", "", "jrb");
		languages.emplace_back ("Judeo-Persian", "", "jpr");
		languages.emplace_back ("Kabardian", "", "kbd", SV { "RU" });
		languages.emplace_back ("Kabyle", "", "kab");
		languages.emplace_back ("Kachin", "", "kac", SV { "MM" });
		languages.emplace_back ("Kalaallisut", "kl", "kal", SV { "GL" });
		languages.emplace_back ("Kalmyk", "", "xal", SV { "RU" });
		languages.emplace_back ("Kamba", "", "kam");
		languages.emplace_back ("Kannada", "kn", "kan", SV { "IN" });
		languages.emplace_back ("Kanuri", "kr", "kau", SV { "NE", "NG" });
		languages.emplace_back ("Kara-Kalpak", "", "kaa", SV { "UZ" });
		languages.emplace_back ("Karachay-Balkar", "", "krc", SV { "RU" });
		languages.emplace_back ("Karelian", "", "krl", SV { "FI" });
		languages.emplace_back ("Kashmiri", "ks", "kas", SV { "IN", "PK" });
		languages.emplace_back ("Kashubian", "", "csb");
		languages.emplace_back ("Kawi", "", "kaw");
		languages.emplace_back ("Kazakh", "kk", "kaz", SV { "KZ", "TM" });
		languages.emplace_back ("Khasi", "", "kha", SV { "IN" });
		languages.emplace_back ("Kikuyu", "", "kik");
		languages.emplace_back ("Kimbundu", "", "kmb", SV { "AO" });
		languages.emplace_back ("Kinyarwanda", "", "kin", SV { "RW" });
		languages.emplace_back ("Central Khmer", "km", "khm", SV { "KH" });
		languages.emplace_back ("Kikuyu", "ki", "kik");
		languages.emplace_back ("Kinyarwanda", "rw", "kin");
		languages.emplace_back ("Kirghiz", "ky", "kir", SV { "KG", "TJ" });
		languages.emplace_back ("Komi", "kv", "kom", SV { "RU" });
		languages.emplace_back ("Kongo", "kg", "kon", SV { "AO" });
		languages.emplace_back ("Konkani", "", "kok", SV { "IN" });
		languages.emplace_back ("Korean", "ko", "kor", SV { "ID", "KP", "KR" });
		languages.emplace_back ("Kosraean", "", "kos", SV { "FM" });
		languages.emplace_back ("Kpelle", "", "kpe", SV { "GN" });
		languages.emplace_back ("Kumyk", "", "kum");
		languages.emplace_back ("Kurdish", "ku", "kur", SV { "IR", "TM" }, true);
		languages.emplace_back ("Kurukh", "", "kru", SV { "IN" });
		languages.emplace_back ("Kutenai", "", "kut");
		languages.emplace_back ("Kuanyama", "kj", "kua", SV { "NA" });
		languages.emplace_back ("Luxembourgish", "lb", "ltz", SV { "LU" });
		languages.emplace_back ("Ganda", "lg", "lug");
		languages.emplace_back ("Ladino", "", "lad");
		languages.emplace_back ("Lahnda", "", "lah");
		languages.emplace_back ("Lamba", "", "lam", SV { "ZM" });
		languages.emplace_back ("Limburgan", "li", "lim");
		languages.emplace_back ("Lingala", "ln", "lin", SV { "CD", "CG" });
		languages.emplace_back ("Lao", "lo", "lao", SV { "LA" });
		languages.emplace_back ("Lithuanian", "lt", "lit", SV { "LT" });
		languages.emplace_back ("Luba-Katanga", "lu", "lub");
		languages.emplace_back ("Luba-Lulua", "", "lua", SV { "CD" });
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Lunda", "", "lun", SV { "ZM" });
		languages.emplace_back ("Luo", "", "luo");
		languages.emplace_back ("Lushai", "", "lus");
		languages.emplace_back ("Latvian", "lv", "lav", SV { "LV" });
		languages.emplace_back ("Lezghian", "", "lez");
		languages.emplace_back ("Lojban", "", "jbo");
		languages.emplace_back ("Low German", "", "nds");
		languages.emplace_back ("Lower Sorbian", "", "dsb");
		languages.emplace_back ("Lozi", "", "loz", SV { "NA", "ZM" });
		languages.emplace_back ("Madurese", "", "mad", SV { "ID" });
		languages.emplace_back ("Magahi", "", "mag");
		languages.emplace_back ("Maithili", "", "mai", SV { "NP", "IN" });
		languages.emplace_back ("Makasar", "", "mak", SV { "ID" });
		languages.emplace_back ("Mandingo", "", "man");
		languages.emplace_back ("Masai", "", "mas");
		languages.emplace_back ("Moksha", "", "mdf", SV { "RU" });
		languages.emplace_back ("Mandar", "", "mdr");
		languages.emplace_back ("Mende", "", "men");
		languages.emplace_back ("Mi'kmaq", "", "mic");
		languages.emplace_back ("Minangkabau", "", "min", SV { "ID" });
		languages.emplace_back ("Manchu", "", "mnc");
		languages.emplace_back ("Manipuri", "", "mni", SV { "IN" });
		languages.emplace_back ("Mohawk", "", "moh");
		languages.emplace_back ("Mossi", "", "mos", SV { "BJ" });
		languages.emplace_back ("Mirandese", "", "mwl", SV { "PT" });
		languages.emplace_back ("Marwari", "", "mwr");
		languages.emplace_back ("Mayan", "", "myn", SV { "MX" });
		languages.emplace_back ("Erzya", "", "myv");
		languages.emplace_back ("Nahuatl", "", "nah", SV { "MX" });
		languages.emplace_back ("Manx", "gv", "glv", SV { "IM" });
		languages.emplace_back ("Macedonian", "mk", "mac", SV { "AL", "MK", "RO" });
		languages.emplace_back ("Malagasy", "mg", "mlg", SV { "MG" });
		languages.emplace_back ("Malay", "ms", "may", SV { "BN", "CX", "CC", "ID", "MY", "SG", "TH" });
		languages.emplace_back ("Malayalam", "ml", "mal", SV { "IN" });
		languages.emplace_back ("Maltese", "mt", "mlt", SV { "MT" });
		languages.emplace_back ("Maori", "mi", "mao", SV { "NZ" });
		languages.emplace_back ("Marathi", "mr", "mar", SV { "IN" });
		languages.emplace_back ("Marshallese", "mh", "mah", SV { "MH" });
		languages.emplace_back ("Mongolian", "mn", "mon", SV { "CN", "MN" });
		languages.emplace_back ("Nauru", "na", "nau", SV { "NR" });
		languages.emplace_back ("Navajo", "nv", "nav", SV { "US" });
		languages.emplace_back ("Neapolitan", "", "nap");
		languages.emplace_back ("Newari", "", "new", SV { "NP" });
		languages.emplace_back ("Nias", "", "nia", SV { "ID" });
		languages.emplace_back ("Niuean", "", "niu", SV { "NU" });
		languages.emplace_back ("Nogai", "", "nog", SV { "RU" });
		languages.emplace_back ("N'Ko", "", "nqo", SV { "GN" });
		languages.emplace_back ("Northern Sotho", "", "nso", SV { "ZA" });
		languages.emplace_back ("Nyamwezi", "", "nym");
		languages.emplace_back ("Nyankole", "", "nyn");
		languages.emplace_back ("Nyoro", "", "nyo");
		languages.emplace_back ("Nzima", "", "nzi", SV { "GH" });
		languages.emplace_back ("North Ndebele", "and", "nde", SV { "ZW" });
		languages.emplace_back ("Nepali", "ne", "nep", SV { "IN", "NP" });
		languages.emplace_back ("Ndonga", "ng", "ndo", SV { "NA" });
		languages.emplace_back ("Norwegian Bokmål", "nb", "nob");
		languages.emplace_back ("Norwegian Nynorsk", "nn", "nno", SV { "NO" });
		languages.emplace_back ("Norwegian", "no", "nor", SV { "BV", "NO" });
		languages.emplace_back ("Sichuan Yi", "ii", "iii");
		languages.emplace_back ("South Ndebele", "nr", "nbl", SV { "ZA" });
		languages.emplace_back ("Occitan", "oc", "oci", SV { "MC" });
		languages.emplace_back ("Ojibwa", "oj", "oji");
		languages.emplace_back ("Oromo", "om", "orm", SV { "ET" });
		languages.emplace_back ("Osage", "", "osa");
		languages.emplace_back ("Oriya", "or", "ori", SV { "IN", "ID" });
		languages.emplace_back ("Ossetian", "os", "oss", SV { "RU" });
		languages.emplace_back ("Papuan", "", "paa");
		languages.emplace_back ("Pangasinan", "", "pag", SV { "PH" });
		languages.emplace_back ("Pahlavi", "", "pal");
		languages.emplace_back ("Pampanga", "", "pam", SV { "PH" });
		languages.emplace_back ("Punjabi", "pa", "pan", SV { "IN", "ID", "PK" });
		languages.emplace_back ("Papiamento", "", "pap", SV { "NL", "CW", "AW" });
		languages.emplace_back ("Palauan", "", "pau", SV { "PW" });
		languages.emplace_back ("Pali", "pi", "pli");
		languages.emplace_back ("Persian", "fa", "per", SV { "IR" }, true);
		languages.emplace_back ("Polish", "pl", "pol", SV { "BY", "CZ", "PL", "RO", "UA" });
		languages.emplace_back ("Pashto", "ps", "pus", SV { "AF", "PK" }, true);
		languages.emplace_back ("Pohnpeian", "", "pon", SV { "FM" });
		languages.emplace_back ("Portuguese", "pt", "por",
								SV { "AO", "BR", "CV", "CO", "GQ", "GY", "MO", "MZ", "PT", "ST", "ZA", "SR", "TL", "UY", "VE" });
		languages.emplace_back ("Quechua", "qu", "que", SV { "AR", "BO", "CO", "EC", "PE" });
		languages.emplace_back ("Rajasthani", "", "raj");
		languages.emplace_back ("Rapanui", "", "rap");
		languages.emplace_back ("Rarotongan", "", "rar");
		languages.emplace_back ("Romani", "", "rom", SV { "NO", "RO", "SE", "MX", "MK", "CZ" });
		languages.emplace_back ("Romansh", "rm", "roh", SV { "CH" });
		languages.emplace_back ("Rundi", "rn", "run", SV { "BI" });
		languages.emplace_back ("Romanian", "ro", "rum", SV { "MD", "RO", "UA" });
		languages.emplace_back ("Russian", "ru", "rus", SV { "AZ", "BY", "CZ", "KZ", "KG", "MD", "RO", "RU", "TJ", "TM", "UA", "UZ" });
		languages.emplace_back ("Sandawe", "", "sad");
		languages.emplace_back ("Yakut", "", "sah", SV { "RU" });
		languages.emplace_back ("Samaritan Aramaic", "", "sam");
		languages.emplace_back ("Sanskrit", "sa", "san", SV { "IN", "ZA" });
		languages.emplace_back ("Sasak", "", "sas", SV { "ID" });
		languages.emplace_back ("Santali", "", "sat", SV { "IN" });
		languages.emplace_back ("Sicilian", "", "scn");
		languages.emplace_back ("Scots", "", "sco", SV { "GB" });
		languages.emplace_back ("Selkup", "", "sel");
		languages.emplace_back ("Shan", "", "shn", SV { "MM" });
		languages.emplace_back ("Sidamo", "", "sid", SV { "ET" });
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Skolt Sami", "", "sms");
		languages.emplace_back ("Sardinian", "sc", "srd", SV { "IT" });
		languages.emplace_back ("Sindhi", "sd", "snd", SV { "IN", "ID", "PK" });
		languages.emplace_back ("Northern Sami", "se", "sme", SV { "FI" });
		languages.emplace_back ("Samoan", "sm", "smo", SV { "AS", "WS" });
		languages.emplace_back ("Sango", "sg", "sag", SV { "CF" });
		languages.emplace_back ("Serbian", "sr", "srp", SV { "BA", "CZ", "MK", "ME", "RO", "RS" });
		languages.emplace_back ("Gaelic", "gd", "gla", SV { "GB" });
		languages.emplace_back ("Shona", "sn", "sna", SV { "ZW" });
		languages.emplace_back ("Sinhala", "si", "sin", SV { "LK" });
		languages.emplace_back ("Soninke", "", "snk", SV { "SN", "MR", "GW", "GM" });
		languages.emplace_back ("Sranan Tongo", "", "srn", SV { "SR" });
		languages.emplace_back ("Serer", "", "srr", SV { "SN", "GW", "GM" });
		languages.emplace_back ("Sukuma", "", "suk", SV { "TZ" });
		languages.emplace_back ("Susu", "", "sus", SV { "GN", "GQ" });
		languages.emplace_back ("Syriac", "", "syr");
		languages.emplace_back ("Slovak", "sk", "slo", SV { "CZ", "RO", "SK", "UA" });
		languages.emplace_back ("Slovenian", "sl", "slv", SV { "AT", "SI" });
		languages.emplace_back ("Somali", "so", "some", SV { "DJ", "ET", "SO" });
		languages.emplace_back ("Sotho", "st", "sot", SV { "LS", "ZA", "ZW" });
		languages.emplace_back ("Spanish", "es", "spa",
								SV { "AR", "BZ", "BO", "CL", "CO", "CR", "CU", "DO", "EC", "", "GQ", "GI", "GT", "GY", "HN", "MX", "NI", "PA", "PY", "PE", "PH", "PR", "ES", "SR", "TT", "US", "UY", "VE", "EH" });
		languages.emplace_back ("Sundanese", "su", "sun", SV { "ID" });
		languages.emplace_back ("Swahili", "sw", "swa", SV { "CD", "KE", "MZ", "RW", "TZ", "UG" });
		languages.emplace_back ("Swati", "ss", "ssw", SV { "SZ", "ZA" });
		languages.emplace_back ("Swedish", "sv", "swe", SV { "AX", "FI", "SE" });
		languages.emplace_back ("Time", "", "tem", SV { "GN" });
		languages.emplace_back ("Tereno", "", "ter");
		languages.emplace_back ("Tetum", "", "tet", SV { "TL" });
		languages.emplace_back ("Tigre", "", "tig", SV { "ER" });
		languages.emplace_back ("Tiv", "", "tiv", SV { "NG" });
		languages.emplace_back ("Tokelau", "", "tkl", SV { "TK" });
		languages.emplace_back ("Tlingit", "", "tli");
		languages.emplace_back ("Tamashek", "", "tmh", SV { "NE" });
		languages.emplace_back ("Tok Pisin", "", "tpi", SV { "PG" });
		languages.emplace_back ("Tsimshian", "", "tsi");
		languages.emplace_back ("Tumbuka", "", "tum", SV { "ZM", "MW" });
		languages.emplace_back ("Tuvalu", "", "tvl", SV { "TV" });
		languages.emplace_back ("Tuvinian", "", "tyv", SV { "RU" });
		languages.emplace_back ("Tamil", "ta", "tam", SV { "IN", "ID", "SG", "ZA", "LK" });
		languages.emplace_back ("Telugu", "te", "tel", SV { "IN", "ID", "ZA" });
		languages.emplace_back ("Tajik", "tg", "tgk", SV { "TJ", "UZ" });
		languages.emplace_back ("Thai", "th", "tha", SV { "TH" });
		languages.emplace_back ("Tigrinya", "ti", "tir", SV { "ER", "ET" });
		languages.emplace_back ("Tibetan", "bo", "tib", SV { "CN" });
		languages.emplace_back ("Turkmen", "tk", "tuk", SV { "IR", "KZ", "TJ", "TM", "UZ" });
		languages.emplace_back ("Tagalog", "tl", "tgl", SV { "PH" });
		languages.emplace_back ("Tswana", "tn", "tsn", SV { "BW", "NA", "ZA", "ZW" });
		languages.emplace_back ("Tonga", "to", "ton", SV { "MW", "TO" });
		languages.emplace_back ("Turkish", "tr", "tur", SV { "CY", "IQ", "MK", "RO", "RU", "TR" });
		languages.emplace_back ("Tsonga", "ts", "tso", SV { "MZ", "ZA", "ZW" });
		languages.emplace_back ("Tatar", "tt", "tat", SV { "KZ", "RU", "TJ", "TM" });
		languages.emplace_back ("Twi", "tw", "twi", SV { "GH" });
		languages.emplace_back ("Tahitian", "ty", "tah", SV { "PF" });
		languages.emplace_back ("Udmurt", "", "udm", SV { "RU" });
		languages.emplace_back ("Ugaritic", "", "uga");
		languages.emplace_back ("Umbundu", "", "umb", SV { "AO" });
		languages.emplace_back ("Uighur", "ug", "uig", SV { "CN", "KZ", "KG", "UZ" });
		languages.emplace_back ("Ukrainian", "uk", "ukr", SV { "BY", "CZ", "KZ", "MD", "RO", "RU", "TJ", "TM", "UA", "UZ" });
		languages.emplace_back ("Urdu", "ur", "urd", SV { "IN", "PK", "ZA" }, true);
		languages.emplace_back ("Uzbek", "uz", "uzb", SV { "KZ", "KG", "TJ", "TM", "UZ" });
		languages.emplace_back ("Vai", "", "vai");
		languages.emplace_back ("Votic", "", "vot");
		languages.emplace_back ("Venda", "ve", "ven", SV { "ZA", "ZW" });
		languages.emplace_back ("Vietnamese", "vi", "vie", SV { "CZ", "VN" });
		languages.emplace_back ("Volapük", "vo", "vol");
		languages.emplace_back ("Walloon", "wa", "wln");
		languages.emplace_back ("Welsh", "cy", "wel", SV { "AR", "GB" });
		languages.emplace_back ("Wolof", "wo", "wol", SV { "GM", "MR", "SN" }, true);
		languages.emplace_back ("Wolaitta", "", "wal", SV { "ET" });
		languages.emplace_back ("Washo", "", "was");
		languages.emplace_back ("Kalmyk", "", "xal");
		languages.emplace_back ("Western Frisian", "fy", "fry", SV { "NL" });
		languages.emplace_back ("Xhosa", "xh", "xho", SV { "ZA", "ZW" });
		languages.emplace_back ("Yao", "", "yao", SV { "MW" });
		languages.emplace_back ("Yapese", "", "yap", SV { "FM" });
		languages.emplace_back ("Yiddish", "yi", "yid", SV { "BY", "RO", "SE", "UA" });
		languages.emplace_back ("Yoruba", "yo", "yor", SV { "BJ", "NG" });
		languages.emplace_back ("Zapotec", "", "zap", SV { "MX" });
		languages.emplace_back ("Zenaga", "", "zen");
		languages.emplace_back ("Standard Moroccan Tamazight", "", "zgh");
		languages.emplace_back ("Zhuang", "za", "zha");
		languages.emplace_back ("Zulu", "zu", "zul", SV { "ZA" });
		languages.emplace_back ("Zuni", "", "zun");
		languages.emplace_back ("Zaza", "", "zza");
	}

	[[nodiscard]] Language getLanguageByName (const std::string_view& name) const
	{
		LIMES_ASSERT (! name.empty());

		return alg::contains_or_default (languages,
										 [&name] (const Language& lang)
										 { return lang.languageName == name; });
	}

	[[nodiscard]] Language getFromISO639_1 (const std::string_view& code) const
	{
		LIMES_ASSERT (code.size() == 2);

		return alg::contains_or_default (languages,
										 [&code] (const Language& lang)
										 { return lang.ISO639_1 == code; });
	}

	[[nodiscard]] Language getFromISO639_2 (const std::string_view& code) const
	{
		LIMES_ASSERT (code.size() == 3);

		return alg::contains_or_default (languages,
										 [&code] (const Language& lang)
										 { return lang.ISO639_2 == code; });
	}

	[[nodiscard]] ds::vector<Language> getLanguagesForCountryCode (const std::string& code) const
	{
		LIMES_ASSERT (code.size() == 2);

		ds::vector<Language> langs;

		for (const auto& lang : languages)
			if (alg::contains (lang.countryCodes, code))
				langs.push_back (lang);

		return langs;
	}

	[[nodiscard]] const ds::vector<Language>& getAll() const
	{
		return languages;
	}

private:

	ds::vector<Language> languages;
};


static const KnownLanguages known_languages;

static const KnownLanguages& getLanguages()
{
	return known_languages;
}


Language getLanguageForCode (const std::string_view& ISOlanguageCode)
{
	if (ISOlanguageCode.length() == 2)
		return getLanguages().getFromISO639_1 (ISOlanguageCode);

	if (ISOlanguageCode.length() == 3)
		return getLanguages().getFromISO639_2 (ISOlanguageCode);

	LIMES_ASSERT_FALSE;

	return {};
}

Language getLanguage (const std::string_view& languageName)
{
	return getLanguages().getLanguageByName (languageName);
}

ds::vector<Language> getLanguagesForCountry (const std::string_view& countryCode)
{
	return getLanguages().getLanguagesForCountryCode (std::string { countryCode });
}

const ds::vector<Language>& getAllKnownLanguages()
{
	return getLanguages().getAll();
}

}  // namespace locale

LIMES_END_NAMESPACE
