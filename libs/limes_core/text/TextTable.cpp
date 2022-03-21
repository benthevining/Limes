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
#include "../misc/Algorithm.h"

namespace limes
{

std::string TextTable::Row::toString (const std::string&	columnSeparator,
									  const vector<size_t>& widths) const
{
	std::string result;

	auto index = 0;

	for (const auto width : widths)
	{
		if (index != 0)
			result += columnSeparator;

		std::string padded;

		if (index < columns.numObjects())
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
	if (newRow || rows.isEmpty())
	{
		newRow = false;
		rows.emplace_back();
	}

	rows.last().columns.push_back (text);
}

TextTable& TextTable::operator<< (const std::string& text)
{
	addColumnToCurrentRow (text);
	return *this;
}

int TextTable::getNumRows() const noexcept
{
	return static_cast<int> (rows.numObjects());
}

int TextTable::getNumColumns() const noexcept
{
	int maxColumns = 0;

	for (const auto& row : rows)
		maxColumns = std::max (maxColumns, static_cast<int> (row.columns.numObjects()));

	return maxColumns;
}

StringVector TextTable::getRows (const std::string& rowPrefix,
								 const std::string& columnSeparator,
								 const std::string& rowSuffix) const
{
	const auto widths = [this]
	{
		vector<size_t> widths_;
		widths_.reserveAndZero (getNumColumns());

		for (const auto& row : rows)
			for (auto i = 0; i < row.columns.numObjects(); ++i)
				widths_[i] = std::max (widths_[i], static_cast<size_t> (row.columns[i].length()));

		return widths_;
	}();

	StringVector outVec;

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

}  // namespace limes
