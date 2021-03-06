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
#include <string_view>
#include <vector>  // for vector
#include "../limes_namespace.h"

/** @file
	This file defines the TextTable class.
	@ingroup text
 */

LIMES_BEGIN_NAMESPACE

namespace text
{

/** A class that creates a table of strings, laid out in rows and columns, and can pretty-print them with the columns aligned.
	@ingroup text
 */
class LIMES_EXPORT TextTable final
{
public:
	/** Appends text to the current row. */
	void addColumnToCurrentRow (const std::string_view& text);

	/** Appends text to the current row. This is identical to calling \c addColumnToCurrentRow() . */
	TextTable& operator<< (const std::string_view& text);

	/** Starts a new row; subsequent calls to \c addTextToCurrentRow() will be added to the next row. */
	void startNewRow();

	/** Starts a new row. This is identical to calling \c startNewRow() . */
	TextTable& operator++();

	/** Clears and resets the table. */
	void clear();

	/** Returns the current number of rows in the table. */
	[[nodiscard]] int getNumRows() const noexcept;

	/** Finds the total number of columns, by looking for the row with the greatest number of columns. */
	[[nodiscard]] int getNumColumns() const noexcept;

	/** Returns a vector with a string for each row.
		The prefix and suffix will be appended/prepended to each row, and and the column separator will be placed between each column.
	 */
	[[nodiscard]] std::vector<std::string> getRows (const std::string_view& rowPrefix		= {},
													const std::string_view& columnSeparator = " ",
													const std::string_view& rowSuffix		= {}) const;

	/** Returns a string containing all the rows in the table.
		The prefix and suffix will be appended/prepended to each row, and and the column separator will be placed between each column.
	 */
	[[nodiscard]] std::string toString (const std::string_view& rowPrefix		= {},
										const std::string_view& columnSeparator = " ",
										const std::string_view& rowSuffix		= {}) const;

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

}  // namespace text

LIMES_END_NAMESPACE
