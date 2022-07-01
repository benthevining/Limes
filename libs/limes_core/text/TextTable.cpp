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

#include "./TextTable.h"
#include <algorithm>  // for max
#include "../limes_namespace.h"
#include "./StringUtils.h"

LIMES_BEGIN_NAMESPACE

namespace text
{

std::string TextTable::Row::toString (const std::string&		 columnSeparator,
									  const std::vector<size_t>& widths) const
{
	std::string result;

	std::vector<std::string>::size_type index = 0;

	for (const auto width : widths)
	{
		if (index != 0)
			result += columnSeparator;

		std::string padded;

		if (index < columns.size())
			padded = columns[index];

		padded.resize (width, ' ');
		result += padded;
		++index;
	}

	return result;
}

/*-----------------------------------------------------------------------------------------------------------------*/

void TextTable::clear()
{
	rows.clear();
	newRow = true;
}

void TextTable::startNewRow()
{
	if (newRow)
		rows.emplace_back();
	else
		newRow = true;
}

TextTable& TextTable::operator++()
{
	startNewRow();
	return *this;
}

void TextTable::addColumnToCurrentRow (const std::string_view& text)
{
	if (newRow || rows.empty())
	{
		newRow = false;
		rows.emplace_back();
	}

	rows.back().columns.push_back (std::string { text });
}

TextTable& TextTable::operator<< (const std::string_view& text)
{
	addColumnToCurrentRow (text);
	return *this;
}

int TextTable::getNumRows() const noexcept
{
	return static_cast<int> (rows.size());
}

int TextTable::getNumColumns() const noexcept
{
	int maxColumns = 0;

	for (const auto& row : rows)
		maxColumns = std::max (maxColumns, static_cast<int> (row.columns.size()));

	return maxColumns;
}

std::vector<std::string> TextTable::getRows (const std::string_view& rowPrefix,
											 const std::string_view& columnSeparator,
											 const std::string_view& rowSuffix) const
{
	const auto widths = [this]
	{
		std::vector<size_t> widths_;
		widths_.resize (static_cast<std::vector<size_t>::size_type> (getNumColumns()));

		for (const auto& row : rows)
			for (std::vector<std::string>::size_type i = 0;
				 i < row.columns.size();
				 ++i)
				widths_[i] = std::max (widths_[i], static_cast<size_t> (row.columns[i].length()));

		return widths_;
	}();

	std::vector<std::string> outVec;

	for (const auto& row : rows)
		outVec.emplace_back (std::string { rowPrefix }.append (row.toString (std::string { columnSeparator }, widths)).append (rowSuffix));	 // cppcheck-suppress useStlAlgorithm

	return outVec;
}

std::string TextTable::toString (const std::string_view& rowPrefix,
								 const std::string_view& columnSeparator,
								 const std::string_view& rowSuffix) const
{
	std::string result;

	const std::string nl { text::new_line };

	for (const auto& row : getRows (rowPrefix, columnSeparator, rowSuffix))
		result += row + nl;	 // cppcheck-suppress useStlAlgorithm

	return result;
}

}  // namespace text

LIMES_END_NAMESPACE
