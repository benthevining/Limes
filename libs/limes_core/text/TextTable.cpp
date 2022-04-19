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

#include "TextTable.h"
#include <algorithm>  // for max
#include <limes_namespace.h>


LIMES_BEGIN_NAMESPACE

std::string TextTable::Row::toString (const std::string&		 columnSeparator,
									  const std::vector<size_t>& widths) const
{
	std::string result;

	auto index = 0;

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

void TextTable::addColumnToCurrentRow (const std::string& text)
{
	if (newRow || rows.empty())
	{
		newRow = false;
		rows.emplace_back();
	}

	rows.back().columns.push_back (text);
}

TextTable& TextTable::operator<< (const std::string& text)
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

std::vector<std::string> TextTable::getRows (const std::string& rowPrefix,
											 const std::string& columnSeparator,
											 const std::string& rowSuffix) const
{
	const auto widths = [this]
	{
		std::vector<size_t> widths_;
		widths_.resize (getNumColumns());

		for (const auto& row : rows)
			for (auto i = 0; i < row.columns.size(); ++i)
				widths_[i] = std::max (widths_[i], static_cast<size_t> (row.columns[i].length()));

		return widths_;
	}();

	std::vector<std::string> outVec;

	for (const auto& row : rows)
		outVec.emplace_back (std::string (rowPrefix).append (row.toString (columnSeparator, widths)).append (rowSuffix));  // cppcheck-suppress useStlAlgorithm

	return outVec;
}

std::string TextTable::toString (const std::string& rowPrefix,
								 const std::string& columnSeparator,
								 const std::string& rowSuffix) const
{
	std::string result;

	for (const auto& row : getRows (rowPrefix, columnSeparator, rowSuffix))
		result += row + "\n";  // cppcheck-suppress useStlAlgorithm

	return result;
}

LIMES_END_NAMESPACE
