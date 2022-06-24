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

#include "./StringUtils.h"
#include <algorithm>		  // for equal
#include <limes_namespace.h>  // for LIMES_BEGIN_NAMESPACE, LIMES_END_...
#include <cctype>			  // for tolower, isspace, toupper
#include <cstring>			  // for strcmp, strlen, size_t
#include <iostream>			  // for string, operator<<, basic_ostream
#include <string>			  // for char_traits, basic_string
#include <vector>			  // for vector
#include <sstream>			  // for stringstream


LIMES_BEGIN_NAMESPACE

namespace text
{

bool isNewline (const std::string_view& string) noexcept
{
	return string == "\n" || string == "\r\n";
}

bool areSame (const char* string1, const char* string2)
{
	return std::strcmp (string1, string2) == 0;
}

bool areSameIgnoringCase (const char* string1, const char* string2)
{
	const auto l1 = length (string1);
	const auto l2 = length (string2);

	if (l1 != l2)
		return false;

	for (auto idx = 0; idx < l1; ++idx)
		if (std::tolower (static_cast<unsigned char> (string1[idx])) != std::tolower (static_cast<unsigned char> (string2[idx])))
			return false;

	return true;
}

bool areSameIgnoringCase (const std::string_view& lhs, const std::string_view& rhs)
{
	return std::equal (lhs.begin(), lhs.end(),
					   rhs.begin(), rhs.end(),
					   [] (char a, char b)
					   {
						   return std::tolower (static_cast<unsigned char> (a)) == std::tolower (static_cast<unsigned char> (b));
					   });
}

int length (const char* string)
{
	return static_cast<int> (std::strlen (string));
}

void trim (std::string& string)
{
	if (string.empty())
		return;

	// trim start
	string.erase (string.begin(), std::find_if (string.begin(), string.end(), [] (unsigned char ch)
												{ return ! std::isspace (ch); }));

	// trim end
	string.erase (std::find_if (string.rbegin(), string.rend(), [] (unsigned char ch)
								{ return ! std::isspace (ch); })
					  .base(),
				  string.end());
}

void dropFirstChars (std::string& string, std::size_t numChars)
{
	string = string.substr (numChars, string.length());
}

void dropLastChars (std::string& string, std::size_t numChars)
{
	string = string.substr (0, string.length() - numChars);
}

std::string quoted (const std::string_view& string)
{
	std::string copy { string };

	if (! copy.starts_with ('"'))
		copy.insert (0, "\"");

	if (! copy.ends_with ('"'))
		copy += '"';

	return copy;
}

std::string unquoted (const std::string_view& string)
{
	std::string copy { string };

	if (copy.starts_with ("\\\""))
		dropFirstChars (copy, 2);
	else if (copy.starts_with ('"'))
		dropFirstChars (copy, 1);

	if (copy.ends_with ("\\\""))
		dropLastChars (copy, 2);
	else if (copy.ends_with ('"'))
		dropLastChars (copy, 1);

	return copy;
}

std::string toUpper (const std::string_view& string)
{
	std::string s { string };

	std::transform (s.begin(), s.end(), s.begin(), [] (auto c)
					{ return static_cast<char> (std::toupper (static_cast<unsigned char> (c))); });

	return s;
}

std::string toLower (const std::string_view& string)
{
	std::string s { string };

	std::transform (s.begin(), s.end(), s.begin(), [] (auto c)
					{ return static_cast<char> (std::tolower (static_cast<unsigned char> (c))); });

	return s;
}

std::string upToFirstOccurrenceOf (const std::string_view& inputString,
								   const std::string_view& stringToFind)
{
	std::string orig { inputString };

	const auto pos = orig.find (stringToFind);

	if (pos == std::string::npos)
		return orig;

	return orig.substr (0, pos);
}

std::string fromFirstOccurrenceOf (const std::string_view& inputString,
								   const std::string_view& stringToFind)
{
	std::string orig { inputString };

	const auto pos = orig.find (stringToFind);

	if (pos == std::string::npos)
		return orig;

	return orig.substr (pos + stringToFind.length(), orig.length());
}

std::vector<std::string> split (const std::string_view& stringToSplit,
								const std::string_view& delimiter,
								bool					includeDelimiterInResults)
{
	const auto delimiterStartChar = delimiter.front();

	std::vector<std::string> tokens;

	auto tokenStart = stringToSplit.begin();
	auto pos		= tokenStart;

	while (pos != stringToSplit.end())
	{
		if (*pos == delimiterStartChar)
		{
			auto delimiterStart = pos++;

			while (pos != stringToSplit.end() && delimiter.find (*pos) != std::string_view::npos)
				++pos;

			if (pos != stringToSplit.begin())
				tokens.push_back ({ tokenStart, includeDelimiterInResults ? pos : delimiterStart });

			tokenStart = pos;
		}
		else
		{
			++pos;
		}
	}

	if (pos != stringToSplit.begin())
		tokens.push_back ({ tokenStart, pos });

	return tokens;
}

std::vector<std::string> splitAtWhitespace (const std::string_view& stringToSplit)
{
	std::vector<std::string> tokens;

	auto tokenStart = stringToSplit.begin();
	auto pos		= tokenStart;

	while (pos != stringToSplit.end())
	{
		if (std::isspace (*pos))
		{
			auto delimiterStart = pos++;

			while (pos != stringToSplit.end() && std::isspace (*pos))
				++pos;

			if (pos != stringToSplit.begin())
				tokens.push_back ({ tokenStart, delimiterStart });

			tokenStart = pos;
		}
		else
		{
			++pos;
		}
	}

	if (pos != stringToSplit.begin())
		tokens.push_back ({ tokenStart, pos });

	for (auto& token : tokens)
		trim (token);

	return tokens;
}

std::vector<std::string> splitAtNewlines (const std::string_view& stringToSplit)
{
	auto tokens = split (stringToSplit, "\n", false);

	// if the newline char was \r\n, then strings will now end with \r
	std::transform (tokens.begin(), tokens.end(), tokens.begin(),
					[] (auto str)
					{
						if (str.ends_with ('\r'))
							return str.substr (0, str.length() - 1);

						return str;
					});

	return tokens;
}

std::string join (const std::vector<std::string>& strings, const std::string_view& delimiter)
{
	if (strings.size() == 1)
		return strings[0];

	std::stringstream stream;

	for (std::vector<std::string>::size_type i = 0; i < strings.size(); ++i)
	{
		const auto& string = strings[i];

		stream << string;

		if ((i + 1 < strings.size()) && (! string.ends_with (delimiter)))
			stream << delimiter;
	}

	return stream.str();
}

std::string joinWithWhitespace (const std::vector<std::string>& strings)
{
	return join (strings, " ");
}

std::string joinWithNewlines (const std::vector<std::string>& strings)
{
	return join (strings, getNewline());
}

std::string getLimesASCII() noexcept
{
	std::stringstream str;

	str << "  __    ____  __  __  ____  ___" << new_line
		<< " (  )  (_  _)(  \\/  )( ___)/ __)" << new_line
		<< "  )(__  _)(_  )    (  )__) \\__ \\" << new_line
		<< " (____)(____)(_/\\/\\_)(____)(___/" << new_line;

	return str.str();
}

void printLimesASCII()
{
	std::cout << getLimesASCII() << std::endl;
}

}  // namespace text

LIMES_END_NAMESPACE
