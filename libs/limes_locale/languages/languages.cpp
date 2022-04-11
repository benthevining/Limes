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
#include <string>		// for basic_string
#include <string_view>	// for string_view
#include <limes_core.h>
#include <limes_vecops.h>  // for LIMES_ASSERT
#include <limes_data_structures.h>


namespace limes::locale
{

struct LanguageData final
{
	LanguageData()
		: languageName (UNKNOWN)
	{
	}

	explicit LanguageData (const std::string_view& name,
						   const std::string_view& ISO_639_1, const std::string_view& ISO_639_2,
						   const StringVector& countries = {})	// NOLINT
		: languageName (name), ISO639_1 (ISO_639_1), ISO639_2 (ISO_639_2), countryCodes (countries)
	{
		LIMES_ASSERT (ISO639_1.empty() || ISO639_1.length() == 2);
		LIMES_ASSERT (ISO639_2.length() == 3);
	}

	[[nodiscard]] bool isValid() const
	{
		return languageName != UNKNOWN && (! ISO639_1.empty() || ! ISO639_2.empty());
	}

	std::string languageName, ISO639_1, ISO639_2;

	StringVector countryCodes;

private:

	static constexpr auto UNKNOWN = "Unknown_language_";
};


/** Note that ISO639-2/B codes are used instead of ISO639-2/T codes.
	See https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
	And https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
 */
struct KnownLanguages final
{
	KnownLanguages()
	{
		languages.emplace_back ("Abkhazian", "ab", "abk", StringVector { "GE" });
		languages.emplace_back ("Achinese", "", "ace", StringVector { "ID" });
		languages.emplace_back ("Acoli", "", "ach", StringVector { "UG" });
		languages.emplace_back ("Adangme", "", "ada", StringVector { "GH" });
		languages.emplace_back ("Adyghe", "", "ady", StringVector { "RU" });
		languages.emplace_back ("Afar", "aa", "aar", StringVector { "DJ", "ER", "ET" });
		languages.emplace_back ("Afrikaans", "af", "afr", StringVector { "NA", "ZA" });
		languages.emplace_back ("Ainu", "", "ain", StringVector { "RU", "JP" });
		languages.emplace_back ("Akan", "ak", "aka", StringVector { "CI", "GH" });
		languages.emplace_back ("Albanian", "sq", "alb", StringVector { "AL", "IT", "MK", "ME", "RO" });
		languages.emplace_back ("Aleut", "", "ale", StringVector { "US", "RU" });
		languages.emplace_back ("Amharic", "am", "amh", StringVector { "ET" });
		languages.emplace_back ("Angika", "", "anp", StringVector { "IN", "NP" });
		languages.emplace_back ("Arabic", "ar", "ara",
								StringVector { "DZ", "BH", "BJ", "TD", "KM", "DJ", "EG", "ER", "GM", "GN", "GW", "ID", "IR", "IL", "JO", "KW", "LB", "LY", "ML", "MR", "MA", "NE", "OM", "PS", "PH", "QA", "SA", "SN", "SO", "ZA", "SD", "SY", "TJ", "TZ", "TN", "AE", "UZ", "VE", "EH", "YE" });
		languages.emplace_back ("Aragonese", "an", "arg", StringVector { "ES" });
		languages.emplace_back ("Arapaho", "", "arp", StringVector { "US" });
		languages.emplace_back ("Arawak", "", "arw", StringVector { "SR", "VE", "GY" });
		languages.emplace_back ("Armenian", "hy", "arm", StringVector { "AM", "CY", "IQ", "RO" });
		languages.emplace_back ("Aromanian", "", "rup", StringVector { "MK" });
		languages.emplace_back ("Assamese", "as", "asm", StringVector { "IN" });
		languages.emplace_back ("Avaric", "av", "ava", StringVector { "RU" });
		languages.emplace_back ("Awadhi", "", "awa", StringVector { "NP" });
		languages.emplace_back ("Aymara", "ay", "aym", StringVector { "BO", "PE" });
		languages.emplace_back ("Azerbaijani", "az", "aze", StringVector { "AZ", "IR", "KZ", "TM", "UZ" });
		languages.emplace_back ("Balinese", "", "ban", StringVector { "ID" });
		languages.emplace_back ("Baluchi", "", "bal", StringVector { "PK", "TM", "IR" });
		languages.emplace_back ("Bambara", "bm", "bam", StringVector { "ML" });
		languages.emplace_back ("Basa", "", "bas", StringVector { "CM" });
		languages.emplace_back ("Bashkir", "ba", "bak", StringVector { "RU" });
		languages.emplace_back ("Basque", "eu", "baq", StringVector { "ES" });
		languages.emplace_back ("Beja", "", "bej", StringVector { "ER" });
		languages.emplace_back ("Belarusian", "be", "bel", StringVector { "BY", "CZ", "UA" });
		languages.emplace_back ("Bemba", "", "bem", StringVector { "ZM" });
		languages.emplace_back ("Bengali", "bn", "ben", StringVector { "BD", "IN", "ID" });
		languages.emplace_back ("Bhojpuri", "", "bho", StringVector { "NP" });
		languages.emplace_back ("Bikol", "", "bik", StringVector { "PH" });
		languages.emplace_back ("Bini", "", "bin", StringVector { "NG" });
		languages.emplace_back ("Bislama", "bi", "bis", StringVector { "VU" });
		languages.emplace_back ("Bilin", "", "byn", StringVector { "ER" });
		languages.emplace_back ("Bokmål", "", "nob", StringVector { "NO" });
		languages.emplace_back ("Bosnian", "bs", "bos", StringVector { "BA", "MK", "ME" });
		languages.emplace_back ("Braj", "", "bra", StringVector { "IN" });
		languages.emplace_back ("Breton", "br", "bre", StringVector { "FR" });
		languages.emplace_back ("Buginese", "", "bug", StringVector { "ID" });
		languages.emplace_back ("Bulgarian", "bg", "bul", StringVector { "BG", "RO", "UA" });
		languages.emplace_back ("Buriat", "", "bua", StringVector { "RU" });
		languages.emplace_back ("Burmese", "my", "bur", StringVector { "MM" });
		languages.emplace_back ("Caddo", "", "cad", StringVector { "US" });
		languages.emplace_back ("Catalan", "ca", "cat", StringVector { "AD", "IT", "ES" });
		languages.emplace_back ("Cebuano", "", "ceb", StringVector { "PH" });
		languages.emplace_back ("Central Khmer", "", "khm", StringVector { "KH" });
		languages.emplace_back ("Chamorro", "ch", "cha", StringVector { "GU", "MP" });
		languages.emplace_back ("Chechen", "ce", "che", StringVector { "RU" });
		languages.emplace_back ("Cherokee", "", "chr", StringVector { "US" });
		languages.emplace_back ("Cheyenne", "", "chy", StringVector { "US" });
		languages.emplace_back ("Chibcha", "", "chb", StringVector { "CO" });
		languages.emplace_back ("Chichewa", "ny", "nya", StringVector { "MW", "MZ", "ZM", "ZW" });
		languages.emplace_back ("Chinese", "zh", "chi", StringVector { "BZ", "BN", "CN", "GY", "HK", "MO", "SR", "TW", "VE" });
		languages.emplace_back ("Chinook jargon", "", "chn", StringVector { "US" });
		languages.emplace_back ("Chipewyan", "", "chp", StringVector { "CA" });
		languages.emplace_back ("Choctaw", "", "cho", StringVector { "US" });
		languages.emplace_back ("Chuukese", "", "chk", StringVector { "FM" });
		languages.emplace_back ("Chuvash", "cv", "chv", StringVector { "RU" });
		languages.emplace_back ("Cornish", "kw", "cor", StringVector { "GB" });
		languages.emplace_back ("Corsican", "co", "cos", StringVector { "FR", "IT" });
		languages.emplace_back ("Cree", "cr", "cre", StringVector { "CA", "US" });
		languages.emplace_back ("Creek", "", "mus", StringVector { "US" });
		languages.emplace_back ("Crimean Tatar", "", "crh", StringVector { "RO", "RU", "UA" });
		languages.emplace_back ("Croatian", "hr", "hrv", StringVector { "AT", "BA", "HR", "ME", "RO" });
		languages.emplace_back ("Czech", "cs", "cze", StringVector { "CZ", "RO" });
		languages.emplace_back ("Dakota", "", "dak", StringVector { "CA", "US" });
		languages.emplace_back ("Danish", "da", "dan", StringVector { "DK", "FO" });
		languages.emplace_back ("Dargwa", "", "dar", StringVector { "RU" });
		languages.emplace_back ("Delaware", "", "del", StringVector { "CA", "US" });
		languages.emplace_back ("Dinka", "", "din", StringVector { "SS" });
		languages.emplace_back ("Divehi", "dv", "div", StringVector { "MV" });
		languages.emplace_back ("Dogri", "", "doi", StringVector { "IN" });
		languages.emplace_back ("Dogrib", "", "dgr", StringVector { "CA" });
		languages.emplace_back ("Duala", "", "dua", StringVector { "CM" });
		languages.emplace_back ("Dutch", "nl", "dut", StringVector { "AW", "BE", "BQ", "CW", "GY", "ID", "NL", "SX", "SR" });
		languages.emplace_back ("Dyula", "", "dyu", StringVector { "GN", "GM", "CI" });
		languages.emplace_back ("Dzongkha", "dz", "dzo", StringVector { "BT" });
		languages.emplace_back ("Efik", "", "efi", StringVector { "NG" });
		languages.emplace_back ("Ekajuk", "", "eka", StringVector { "NG" });
		languages.emplace_back ("English", "en", "eng",
								StringVector { "AS", "AI", "AG", "AU", "AZ", "BS", "BH", "BB", "BZ", "BJ", "BM", "BQ", "BW", "IO", "BN", "BI", "CM", "CA", "KY", "CX", "CC", "CO", "CK", "CU", "CW", "DM", "ER", "SZ", "FK", "FJ", "FI", "GM", "GH", "GI", "GD", "GU", "GG", "GW", "GN", "GY",
											   "HK", "IN", "ID", "IQ", "IE", "JM", "KE", "KI", "LS", "LR", "MW", "MY", "MV", "MT", "MH", "MU", "FM", "NA", "NR", "NL", "NZ", "NI", "NG", "NU", "NF", "MP", "PK", "PW", "PG", "PH", "PN", "PR", "QA", "RU", "RW", "KN", "LC", "VC", "WS", "SC", "SL", "SG", "SX", "SB", "SO", "ZA", "GS", "SS", "LK", "SD", "SR", "TZ", "TH", "TL", "TK", "TO", "TT", "TC", "TV", "UG", "GB", "US", "VU", "VE", "VG", "VI", "ZM", "ZW" });
		languages.emplace_back ("Erzya", "", "myv", StringVector { "RU" });
		languages.emplace_back ("Esperanto", "eo", "epo");
		languages.emplace_back ("Estonian", "et", "est", StringVector { "EE" });
		languages.emplace_back ("Ewe", "ee", "ewe", StringVector { "GH", "TG" });
		languages.emplace_back ("Ewondo", "", "ewo", StringVector { "CM" });
		languages.emplace_back ("Fang", "", "fan", StringVector { "GA", "GQ" });
		languages.emplace_back ("Fanti", "", "fat", StringVector { "GH" });
		languages.emplace_back ("Faroese", "fo", "fao", StringVector { "FO" });
		languages.emplace_back ("Fijian", "fj", "fij", StringVector { "FJ" });
		languages.emplace_back ("Filipino", "", "fil", StringVector { "PH" });
		languages.emplace_back ("Finnish", "fi", "fin", StringVector { "FI", "SE" });
		languages.emplace_back ("Fon", "", "fon", StringVector { "BJ" });
		languages.emplace_back ("French", "fr", "fre",
								StringVector { "DZ", "BE", "BJ", "BF", "BI", "CM", "CA", "CF", "TD", "KM", "CD", "CG", "CI", "DJ", "GQ", "FR", "PF", "GM", "GN", "GY", "HT", "VA", "LA", "LB", "LU", "MG", "ML", "MR", "MU", "MC", "MA", "NC", "NE", "RU", "RW", "BL", "MF", "SN", "SC", "SR", "CH", "TG", "TN", "VU", "WF", "EH" });
		languages.emplace_back ("Friulian", "", "fur", StringVector { "IT" });
		languages.emplace_back ("Fulah", "ff", "ful", StringVector { "BJ", "CM", "GM", "GN", "ML", "NE", "NG" });
		languages.emplace_back ("Ga", "", "gaa", StringVector { "GH" });
		languages.emplace_back ("Carib", "", "car", StringVector { "SR", "VE", "GY" });
		languages.emplace_back ("Galician", "gl", "glg", StringVector { "ES" });
		languages.emplace_back ("Ganda", "", "lug", StringVector { "UG" });
		languages.emplace_back ("Gayo", "", "gay", StringVector { "ID" });
		languages.emplace_back ("Gbaya", "", "gba", StringVector { "CD", "CF", "CM", "CG" });
		languages.emplace_back ("Georgian", "ka", "geo", StringVector { "GE" });
		languages.emplace_back ("German", "de", "ger",
								StringVector { "AT", "BE", "BZ", "CR", "CZ", "DK", "DE", "LI", "LU", "NA", "RO", "RU", "ZA", "CH", "VE" });
		languages.emplace_back ("Gilbertese", "", "gil", StringVector { "KI" });
		languages.emplace_back ("Gondi", "", "gon", StringVector { "IN" });
		languages.emplace_back ("Gorontalo", "", "gor", StringVector { "ID" });
		languages.emplace_back ("Grebo", "", "grb", StringVector { "LR" });
		languages.emplace_back ("Greek", "el", "gre", StringVector { "AL", "CY", "CZ", "GR", "RO", "ZA", "UA" });
		languages.emplace_back ("Guarani", "gn", "grn", StringVector { "AR", "BO", "PY" });
		languages.emplace_back ("Gujarati", "gu", "guj", StringVector { "IN", "ID", "ZA" });
		languages.emplace_back ("Gwich'in", "", "gwi", StringVector { "CA", "US" });
		languages.emplace_back ("Haida", "", "hai", StringVector { "CA" });
		languages.emplace_back ("Haitian", "ht", "hat", StringVector { "CU", "HT" });
		languages.emplace_back ("Hausa", "ha", "hau", StringVector { "BJ", "NE", "NG" });
		languages.emplace_back ("Hawaiian", "", "haw", StringVector { "US" });
		languages.emplace_back ("Hebrew", "he", "heb", StringVector { "IL", "ZA", "UA" });
		languages.emplace_back ("Herero", "hz", "her", StringVector { "NA" });
		languages.emplace_back ("Hiligaynon", "", "hil", StringVector { "PH" });
		languages.emplace_back ("Hindi", "hi", "hin", StringVector { "IN", "ZA" });
		languages.emplace_back ("Hiri Motu", "ho", "hmo", StringVector { "PG" });
		languages.emplace_back ("Hmong", "", "hmn", StringVector { "LA" });
		languages.emplace_back ("Hungarian", "hu", "hun", StringVector { "AT", "CZ", "HU", "RO", "SI", "UA" });
		languages.emplace_back ("Hupa", "", "hup");
		languages.emplace_back ("Iban", "", "iba");
		languages.emplace_back ("Iloko", "", "ilo", StringVector { "PH" });
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Ingush", "", "inh", StringVector { "RU" });
		languages.emplace_back ("Interlingua", "ia", "ina");
		languages.emplace_back ("Indonesian", "id", "ind", StringVector { "ID", "TL" });
		languages.emplace_back ("Interlingue", "ie", "ile");
		languages.emplace_back ("Irish", "ga", "gle", StringVector { "IE", "GB" });
		languages.emplace_back ("Igbo", "ig", "ibo", StringVector { "NG" });
		languages.emplace_back ("Inupiaq", "ik", "ipk");
		languages.emplace_back ("Ido", "io", "ido");
		languages.emplace_back ("Icelandic", "is", "ice", StringVector { "IS" });
		languages.emplace_back ("Italian", "it", "ita", StringVector { "CR", "VA", "IT", "MT", "MC", "RO", "SM", "SI", "SO", "CH", "VE" });
		languages.emplace_back ("Inuktitut", "iu", "iku");
		languages.emplace_back ("Inupiaq", "", "ipk");
		languages.emplace_back ("Japanese", "ja", "jpn", StringVector { "ID", "JP", "PW" });
		languages.emplace_back ("Javanese", "jv", "jav", StringVector { "NC", "SR" });
		languages.emplace_back ("Judeo-Arabic", "", "jrb");
		languages.emplace_back ("Judeo-Persian", "", "jpr");
		languages.emplace_back ("Kabardian", "", "kbd", StringVector { "RU" });
		languages.emplace_back ("Kabyle", "", "kab");
		languages.emplace_back ("Kachin", "", "kac", StringVector { "MM" });
		languages.emplace_back ("Kalaallisut", "kl", "kal", StringVector { "GL" });
		languages.emplace_back ("Kalmyk", "", "xal", StringVector { "RU" });
		languages.emplace_back ("Kamba", "", "kam");
		languages.emplace_back ("Kannada", "kn", "kan", StringVector { "IN" });
		languages.emplace_back ("Kanuri", "kr", "kau", StringVector { "NE", "NG" });
		languages.emplace_back ("Kara-Kalpak", "", "kaa", StringVector { "UZ" });
		languages.emplace_back ("Karachay-Balkar", "", "krc", StringVector { "RU" });
		languages.emplace_back ("Karelian", "", "krl", StringVector { "FI" });
		languages.emplace_back ("Kashmiri", "ks", "kas", StringVector { "IN", "PK" });
		languages.emplace_back ("Kashubian", "", "csb");
		languages.emplace_back ("Kawi", "", "kaw");
		languages.emplace_back ("Kazakh", "kk", "kaz", StringVector { "KZ", "TM" });
		languages.emplace_back ("Khasi", "", "kha", StringVector { "IN" });
		languages.emplace_back ("Kikuyu", "", "kik");
		languages.emplace_back ("Kimbundu", "", "kmb", StringVector { "AO" });
		languages.emplace_back ("Kinyarwanda", "", "kin", StringVector { "RW" });
		languages.emplace_back ("Central Khmer", "km", "khm", StringVector { "KH" });
		languages.emplace_back ("Kikuyu", "ki", "kik");
		languages.emplace_back ("Kinyarwanda", "rw", "kin");
		languages.emplace_back ("Kirghiz", "ky", "kir", StringVector { "KG", "TJ" });
		languages.emplace_back ("Komi", "kv", "kom", StringVector { "RU" });
		languages.emplace_back ("Kongo", "kg", "kon", StringVector { "AO" });
		languages.emplace_back ("Konkani", "", "kok", StringVector { "IN" });
		languages.emplace_back ("Korean", "ko", "kor", StringVector { "ID", "KP", "KR" });
		languages.emplace_back ("Kosraean", "", "kos", StringVector { "FM" });
		languages.emplace_back ("Kpelle", "", "kpe", StringVector { "GN" });
		languages.emplace_back ("Kumyk", "", "kum");
		languages.emplace_back ("Kurdish", "ku", "kur", StringVector { "IR", "TM" });
		languages.emplace_back ("Kurukh", "", "kru", StringVector { "IN" });
		languages.emplace_back ("Kutenai", "", "kut");
		languages.emplace_back ("Kuanyama", "kj", "kua", StringVector { "NA" });
		languages.emplace_back ("Luxembourgish", "lb", "ltz", StringVector { "LU" });
		languages.emplace_back ("Ganda", "lg", "lug");
		languages.emplace_back ("Ladino", "", "lad");
		languages.emplace_back ("Lahnda", "", "lah");
		languages.emplace_back ("Lamba", "", "lam", StringVector { "ZM" });
		languages.emplace_back ("Limburgan", "li", "lim");
		languages.emplace_back ("Lingala", "ln", "lin", StringVector { "CD", "CG" });
		languages.emplace_back ("Lao", "lo", "lao", StringVector { "LA" });
		languages.emplace_back ("Lithuanian", "lt", "lit", StringVector { "LT" });
		languages.emplace_back ("Luba-Katanga", "lu", "lub");
		languages.emplace_back ("Luba-Lulua", "", "lua", StringVector { "CD" });
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Lunda", "", "lun", StringVector { "ZM" });
		languages.emplace_back ("Luo", "", "luo");
		languages.emplace_back ("Lushai", "", "lus");
		languages.emplace_back ("Latvian", "lv", "lav", StringVector { "LV" });
		languages.emplace_back ("Lezghian", "", "lez");
		languages.emplace_back ("Lojban", "", "jbo");
		languages.emplace_back ("Low German", "", "nds");
		languages.emplace_back ("Lower Sorbian", "", "dsb");
		languages.emplace_back ("Lozi", "", "loz", StringVector { "NA", "ZM" });
		languages.emplace_back ("Madurese", "", "mad", StringVector { "ID" });
		languages.emplace_back ("Magahi", "", "mag");
		languages.emplace_back ("Maithili", "", "mai", StringVector { "NP", "IN" });
		languages.emplace_back ("Makasar", "", "mak", StringVector { "ID" });
		languages.emplace_back ("Mandingo", "", "man");
		languages.emplace_back ("Masai", "", "mas");
		languages.emplace_back ("Moksha", "", "mdf", StringVector { "RU" });
		languages.emplace_back ("Mandar", "", "mdr");
		languages.emplace_back ("Mende", "", "men");
		languages.emplace_back ("Mi'kmaq", "", "mic");
		languages.emplace_back ("Minangkabau", "", "min", StringVector { "ID" });
		languages.emplace_back ("Manchu", "", "mnc");
		languages.emplace_back ("Manipuri", "", "mni", StringVector { "IN" });
		languages.emplace_back ("Mohawk", "", "moh");
		languages.emplace_back ("Mossi", "", "mos", StringVector { "BJ" });
		languages.emplace_back ("Mirandese", "", "mwl", StringVector { "PT" });
		languages.emplace_back ("Marwari", "", "mwr");
		languages.emplace_back ("Mayan", "", "myn", StringVector { "MX" });
		languages.emplace_back ("Erzya", "", "myv");
		languages.emplace_back ("Nahuatl", "", "nah", StringVector { "MX" });
		languages.emplace_back ("Manx", "gv", "glv", StringVector { "IM" });
		languages.emplace_back ("Macedonian", "mk", "mac", StringVector { "AL", "MK", "RO" });
		languages.emplace_back ("Malagasy", "mg", "mlg", StringVector { "MG" });
		languages.emplace_back ("Malay", "ms", "may", StringVector { "BN", "CX", "CC", "ID", "MY", "SG", "TH" });
		languages.emplace_back ("Malayalam", "ml", "mal", StringVector { "IN" });
		languages.emplace_back ("Maltese", "mt", "mlt", StringVector { "MT" });
		languages.emplace_back ("Maori", "mi", "mao", StringVector { "NZ" });
		languages.emplace_back ("Marathi", "mr", "mar", StringVector { "IN" });
		languages.emplace_back ("Marshallese", "mh", "mah", StringVector { "MH" });
		languages.emplace_back ("Mongolian", "mn", "mon", StringVector { "CN", "MN" });
		languages.emplace_back ("Nauru", "na", "nau", StringVector { "NR" });
		languages.emplace_back ("Navajo", "nv", "nav", StringVector { "US" });
		languages.emplace_back ("Neapolitan", "", "nap");
		languages.emplace_back ("Newari", "", "new", StringVector { "NP" });
		languages.emplace_back ("Nias", "", "nia", StringVector { "ID" });
		languages.emplace_back ("Niuean", "", "niu", StringVector { "NU" });
		languages.emplace_back ("Nogai", "", "nog", StringVector { "RU" });
		languages.emplace_back ("N'Ko", "", "nqo", StringVector { "GN" });
		languages.emplace_back ("Northern Sotho", "", "nso", StringVector { "ZA" });
		languages.emplace_back ("Nyamwezi", "", "nym");
		languages.emplace_back ("Nyankole", "", "nyn");
		languages.emplace_back ("Nyoro", "", "nyo");
		languages.emplace_back ("Nzima", "", "nzi", StringVector { "GH" });
		languages.emplace_back ("North Ndebele", "and", "nde", StringVector { "ZW" });
		languages.emplace_back ("Nepali", "ne", "nep", StringVector { "IN", "NP" });
		languages.emplace_back ("Ndonga", "ng", "ndo", StringVector { "NA" });
		languages.emplace_back ("Norwegian Bokmål", "nb", "nob");
		languages.emplace_back ("Norwegian Nynorsk", "nn", "nno", StringVector { "NO" });
		languages.emplace_back ("Norwegian", "no", "nor", StringVector { "BV", "NO" });
		languages.emplace_back ("Sichuan Yi", "ii", "iii");
		languages.emplace_back ("South Ndebele", "nr", "nbl", StringVector { "ZA" });
		languages.emplace_back ("Occitan", "oc", "oci", StringVector { "MC" });
		languages.emplace_back ("Ojibwa", "oj", "oji");
		languages.emplace_back ("Oromo", "om", "orm", StringVector { "ET" });
		languages.emplace_back ("Osage", "", "osa");
		languages.emplace_back ("Oriya", "or", "ori", StringVector { "IN", "ID" });
		languages.emplace_back ("Ossetian", "os", "oss", StringVector { "RU" });
		languages.emplace_back ("Papuan", "", "paa");
		languages.emplace_back ("Pangasinan", "", "pag", StringVector { "PH" });
		languages.emplace_back ("Pahlavi", "", "pal");
		languages.emplace_back ("Pampanga", "", "pam", StringVector { "PH" });
		languages.emplace_back ("Punjabi", "pa", "pan", StringVector { "IN", "ID", "PK" });
		languages.emplace_back ("Papiamento", "", "pap", StringVector { "NL", "CW", "AW" });
		languages.emplace_back ("Palauan", "", "pau", StringVector { "PW" });
		languages.emplace_back ("Pali", "pi", "pli");
		languages.emplace_back ("Persian", "fa", "per", StringVector { "IR" });
		languages.emplace_back ("Polish", "pl", "pol", StringVector { "BY", "CZ", "PL", "RO", "UA" });
		languages.emplace_back ("Pashto", "ps", "pus", StringVector { "AF", "PK" });
		languages.emplace_back ("Pohnpeian", "", "pon", StringVector { "FM" });
		languages.emplace_back ("Portuguese", "pt", "por",
								StringVector { "AO", "BR", "CV", "CO", "GQ", "GY", "MO", "MZ", "PT", "ST", "ZA", "SR", "TL", "UY", "VE" });
		languages.emplace_back ("Quechua", "qu", "que", StringVector { "AR", "BO", "CO", "EC", "PE" });
		languages.emplace_back ("Rajasthani", "", "raj");
		languages.emplace_back ("Rapanui", "", "rap");
		languages.emplace_back ("Rarotongan", "", "rar");
		languages.emplace_back ("Romani", "", "rom", StringVector { "NO", "RO", "SE", "MX", "MK", "CZ" });
		languages.emplace_back ("Romansh", "rm", "roh", StringVector { "CH" });
		languages.emplace_back ("Rundi", "rn", "run", StringVector { "BI" });
		languages.emplace_back ("Romanian", "ro", "rum", StringVector { "MD", "RO", "UA" });
		languages.emplace_back ("Russian", "ru", "rus", StringVector { "AZ", "BY", "CZ", "KZ", "KG", "MD", "RO", "RU", "TJ", "TM", "UA", "UZ" });
		languages.emplace_back ("Sandawe", "", "sad");
		languages.emplace_back ("Yakut", "", "sah", StringVector { "RU" });
		languages.emplace_back ("Samaritan Aramaic", "", "sam");
		languages.emplace_back ("Sanskrit", "sa", "san", StringVector { "IN", "ZA" });
		languages.emplace_back ("Sasak", "", "sas", StringVector { "ID" });
		languages.emplace_back ("Santali", "", "sat", StringVector { "IN" });
		languages.emplace_back ("Sicilian", "", "scn");
		languages.emplace_back ("Scots", "", "sco", StringVector { "GB" });
		languages.emplace_back ("Selkup", "", "sel");
		languages.emplace_back ("Shan", "", "shn", StringVector { "MM" });
		languages.emplace_back ("Sidamo", "", "sid", StringVector { "ET" });
		languages.emplace_back ("Lule Sami", "", "smj");
		languages.emplace_back ("Inari Sami", "", "smn");
		languages.emplace_back ("Skolt Sami", "", "sms");
		languages.emplace_back ("Sardinian", "sc", "srd", StringVector { "IT" });
		languages.emplace_back ("Sindhi", "sd", "snd", StringVector { "IN", "ID", "PK" });
		languages.emplace_back ("Northern Sami", "se", "sme", StringVector { "FI" });
		languages.emplace_back ("Samoan", "sm", "smo", StringVector { "AS", "WS" });
		languages.emplace_back ("Sango", "sg", "sag", StringVector { "CF" });
		languages.emplace_back ("Serbian", "sr", "srp", StringVector { "BA", "CZ", "MK", "ME", "RO", "RS" });
		languages.emplace_back ("Gaelic", "gd", "gla", StringVector { "GB" });
		languages.emplace_back ("Shona", "sn", "sna", StringVector { "ZW" });
		languages.emplace_back ("Sinhala", "si", "sin", StringVector { "LK" });
		languages.emplace_back ("Soninke", "", "snk", StringVector { "SN", "MR", "GW", "GM" });
		languages.emplace_back ("Sranan Tongo", "", "srn", StringVector { "SR" });
		languages.emplace_back ("Serer", "", "srr", StringVector { "SN", "GW", "GM" });
		languages.emplace_back ("Sukuma", "", "suk", StringVector { "TZ" });
		languages.emplace_back ("Susu", "", "sus", StringVector { "GN", "GQ" });
		languages.emplace_back ("Syriac", "", "syr");
		languages.emplace_back ("Slovak", "sk", "slo", StringVector { "CZ", "RO", "SK", "UA" });
		languages.emplace_back ("Slovenian", "sl", "slv", StringVector { "AT", "SI" });
		languages.emplace_back ("Somali", "so", "some", StringVector { "DJ", "ET", "SO" });
		languages.emplace_back ("Sotho", "st", "sot", StringVector { "LS", "ZA", "ZW" });
		languages.emplace_back ("Spanish", "es", "spa",
								StringVector { "AR", "BZ", "BO", "CL", "CO", "CR", "CU", "DO", "EC", "SV", "GQ", "GI", "GT", "GY", "HN", "MX", "NI", "PA", "PY", "PE", "PH", "PR", "ES", "SR", "TT", "US", "UY", "VE", "EH" });
		languages.emplace_back ("Sundanese", "su", "sun", StringVector { "ID" });
		languages.emplace_back ("Swahili", "sw", "swa", StringVector { "CD", "KE", "MZ", "RW", "TZ", "UG" });
		languages.emplace_back ("Swati", "ss", "ssw", StringVector { "SZ", "ZA" });
		languages.emplace_back ("Swedish", "sv", "swe", StringVector { "AX", "FI", "SE" });
		languages.emplace_back ("Time", "", "tem", StringVector { "GN" });
		languages.emplace_back ("Tereno", "", "ter");
		languages.emplace_back ("Tetum", "", "tet", StringVector { "TL" });
		languages.emplace_back ("Tigre", "", "tig", StringVector { "ER" });
		languages.emplace_back ("Tiv", "", "tiv", StringVector { "NG" });
		languages.emplace_back ("Tokelau", "", "tkl", StringVector { "TK" });
		languages.emplace_back ("Tlingit", "", "tli");
		languages.emplace_back ("Tamashek", "", "tmh", StringVector { "NE" });
		languages.emplace_back ("Tok Pisin", "", "tpi", StringVector { "PG" });
		languages.emplace_back ("Tsimshian", "", "tsi");
		languages.emplace_back ("Tumbuka", "", "tum", StringVector { "ZM", "MW" });
		languages.emplace_back ("Tuvalu", "", "tvl", StringVector { "TV" });
		languages.emplace_back ("Tuvinian", "", "tyv", StringVector { "RU" });
		languages.emplace_back ("Tamil", "ta", "tam", StringVector { "IN", "ID", "SG", "ZA", "LK" });
		languages.emplace_back ("Telugu", "te", "tel", StringVector { "IN", "ID", "ZA" });
		languages.emplace_back ("Tajik", "tg", "tgk", StringVector { "TJ", "UZ" });
		languages.emplace_back ("Thai", "th", "tha", StringVector { "TH" });
		languages.emplace_back ("Tigrinya", "ti", "tir", StringVector { "ER", "ET" });
		languages.emplace_back ("Tibetan", "bo", "tib", StringVector { "CN" });
		languages.emplace_back ("Turkmen", "tk", "tuk", StringVector { "IR", "KZ", "TJ", "TM", "UZ" });
		languages.emplace_back ("Tagalog", "tl", "tgl", StringVector { "PH" });
		languages.emplace_back ("Tswana", "tn", "tsn", StringVector { "BW", "NA", "ZA", "ZW" });
		languages.emplace_back ("Tonga", "to", "ton", StringVector { "MW", "TO" });
		languages.emplace_back ("Turkish", "tr", "tur", StringVector { "CY", "IQ", "MK", "RO", "RU", "TR" });
		languages.emplace_back ("Tsonga", "ts", "tso", StringVector { "MZ", "ZA", "ZW" });
		languages.emplace_back ("Tatar", "tt", "tat", StringVector { "KZ", "RU", "TJ", "TM" });
		languages.emplace_back ("Twi", "tw", "twi", StringVector { "GH" });
		languages.emplace_back ("Tahitian", "ty", "tah", StringVector { "PF" });
		languages.emplace_back ("Udmurt", "", "udm", StringVector { "RU" });
		languages.emplace_back ("Ugaritic", "", "uga");
		languages.emplace_back ("Umbundu", "", "umb", StringVector { "AO" });
		languages.emplace_back ("Uighur", "ug", "uig", StringVector { "CN", "KZ", "KG", "UZ" });
		languages.emplace_back ("Ukrainian", "uk", "ukr", StringVector { "BY", "CZ", "KZ", "MD", "RO", "RU", "TJ", "TM", "UA", "UZ" });
		languages.emplace_back ("Urdu", "ur", "urd", StringVector { "IN", "PK", "ZA" });
		languages.emplace_back ("Uzbek", "uz", "uzb", StringVector { "KZ", "KG", "TJ", "TM", "UZ" });
		languages.emplace_back ("Vai", "", "vai");
		languages.emplace_back ("Votic", "", "vot");
		languages.emplace_back ("Venda", "ve", "ven", StringVector { "ZA", "ZW" });
		languages.emplace_back ("Vietnamese", "vi", "vie", StringVector { "CZ", "VN" });
		languages.emplace_back ("Volapük", "vo", "vol");
		languages.emplace_back ("Walloon", "wa", "wln");
		languages.emplace_back ("Welsh", "cy", "wel", StringVector { "AR", "GB" });
		languages.emplace_back ("Wolof", "wo", "wol", StringVector { "GM", "MR", "SN" });
		languages.emplace_back ("Wolaitta", "", "wal", StringVector { "ET" });
		languages.emplace_back ("Washo", "", "was");
		languages.emplace_back ("Kalmyk", "", "xal");
		languages.emplace_back ("Western Frisian", "fy", "fry", StringVector { "NL" });
		languages.emplace_back ("Xhosa", "xh", "xho", StringVector { "ZA", "ZW" });
		languages.emplace_back ("Yao", "", "yao", StringVector { "MW" });
		languages.emplace_back ("Yapese", "", "yap", StringVector { "FM" });
		languages.emplace_back ("Yiddish", "yi", "yid", StringVector { "BY", "RO", "SE", "UA" });
		languages.emplace_back ("Yoruba", "yo", "yor", StringVector { "BJ", "NG" });
		languages.emplace_back ("Zapotec", "", "zap", StringVector { "MX" });
		languages.emplace_back ("Zenaga", "", "zen");
		languages.emplace_back ("Standard Moroccan Tamazight", "", "zgh");
		languages.emplace_back ("Zhuang", "za", "zha");
		languages.emplace_back ("Zulu", "zu", "zul", StringVector { "ZA" });
		languages.emplace_back ("Zuni", "", "zun");
		languages.emplace_back ("Zaza", "", "zza");
	}

	[[nodiscard]] LanguageData getDataForLanguageName (const std::string& name) const
	{
		LIMES_ASSERT (! name.empty());

		return alg::contains_or_default (languages,
										 [&name] (const LanguageData& lang)
										 { return lang.languageName == name; });
	}

	[[nodiscard]] LanguageData getDataForISO639_1 (const std::string& code) const
	{
		LIMES_ASSERT (code.size() == 2);

		return alg::contains_or_default (languages,
										 [&code] (const LanguageData& lang)
										 { return lang.ISO639_1 == code; });
	}

	[[nodiscard]] LanguageData getDataForISO639_2 (const std::string& code) const
	{
		LIMES_ASSERT (code.size() == 3);

		return alg::contains_or_default (languages,
										 [&code] (const LanguageData& lang)
										 { return lang.ISO639_2 == code; });
	}

	[[nodiscard]] StringVector getLanguageNamesForCountryCode (const std::string& code) const
	{
		LIMES_ASSERT (code.size() == 2);

		StringVector langs;

		for (const auto& lang : languages)
			if (alg::contains (lang.countryCodes, code))
				langs.push_back (lang.languageName);

		return langs;
	}

	[[nodiscard]] StringVector getLanguageCodesForCountryCode (const std::string& code, bool asISO639_2) const
	{
		LIMES_ASSERT (code.size() == 2);

		StringVector langs;

		for (const auto& lang : languages)
		{
			if (alg::contains (lang.countryCodes, code))
			{
				if (asISO639_2)
					langs.push_back (lang.ISO639_2);
				else
					langs.push_back (lang.ISO639_1);
			}
		}

		return langs;
	}

private:

	vector<LanguageData> languages;
};


static const KnownLanguages& getLanguages()
{
	static const KnownLanguages languages;

	return languages;
}


std::string languageNameToCode (const std::string& name, bool asISO639_2)
{
	const auto data = getLanguages().getDataForLanguageName (name);

	if (! data.isValid())
		return {};

	if (asISO639_2)
		return data.ISO639_2;

	return data.ISO639_1;
}

std::string languageCodeToName (const std::string& languageCode)
{
	const auto data = [languageCode]() -> LanguageData
	{
		if (languageCode.length() == 2)
			return getLanguages().getDataForISO639_1 (languageCode);

		if (languageCode.length() == 3)
			return getLanguages().getDataForISO639_2 (languageCode);

		return {};
	}();

	if (! data.isValid())
		return {};

	return data.languageName;
}

StringVector getCountryCodesForLanguage (const std::string& language, bool languageName)
{
	const auto language_name = [languageName, language]
	{
		if (languageName)
			return language;

		return languageCodeToName (language);
	}();

	const auto data = getLanguages().getDataForLanguageName (language_name);

	if (! data.isValid())
		return {};

	return data.countryCodes;
}

StringVector getLanguageNamesForCountryCode (const std::string& countryCode)
{
	if (countryCode.length() != 2)
		return {};

	return getLanguages().getLanguageNamesForCountryCode (countryCode);
}

StringVector getLanguageCodesForCountry (const std::string& countryCode, bool asISO639_2)
{
	if (countryCode.length() != 2)
		return {};

	return getLanguages().getLanguageCodesForCountryCode (countryCode, asISO639_2);
}

}  // namespace limes::locale
