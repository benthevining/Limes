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

#include <limes_export.h>  // for LIMES_EXPORT
#include <cstring>		   // for size_t
#include <string>		   // for basic_string
#include <vector>		   // for vector

namespace limes
{

class LIMES_EXPORT TextTable final
{
public:

	/** Appends text to the current row.
	 */
	void addColumnToCurrentRow (const std::string& text);

	/** Appends text to the current row. This is identical to calling addColumnToCurrentRow().
	 */
	TextTable& operator<< (const std::string& text);

	/** Starts a new row; subsequent calls to addTextToCurrentRow() will be added to the next row.
	 */
	void startNewRow();

	/** Starts a new row. This is identical to calling startNewRow(). */
	TextTable& operator++();

	/** Clears and resets the table. */
	void clear();

	/** Returns the current number of rows in the table. */
	[[nodiscard]] int getNumRows() const noexcept;

	/** Finds the total number of columns, by looking for the row with the greatest number of columns. */
	[[nodiscard]] int getNumColumns() const noexcept;

	/** Returns a vector with a string for each row.
		The prefix and suffix will be appended to each row, and and the column separator will be placed between each column.
	 */
	[[nodiscard]] std::vector<std::string> getRows (const std::string& rowPrefix,
													const std::string& columnSeparator,
													const std::string& rowSuffix) const;

	/** Returns a string containing all the rows in the table.
	 */
	[[nodiscard]] std::string toString (const std::string& rowPrefix,
										const std::string& columnSeparator,
										const std::string& rowSuffix) const;

private:

	struct Row final
	{
		[[nodiscard]] std::string toString (const std::string&		   columnSeparator,
											const std::vector<size_t>& widths) const;

		std::vector<std::string> columns;
	};

	std::vector<Row> rows;

	bool newRow { true };
};

}  // namespace limes
