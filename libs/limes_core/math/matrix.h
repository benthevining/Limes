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

#include "../limes_namespace.h"
#include <limes_export.h>
#include "../system/limes_assert.h"
#include <array>
#include <functional>

/** @file
	This file defines the Matrix class.
	@ingroup core_math
 */

LIMES_BEGIN_NAMESPACE

namespace math
{

/** This class is a %matrix, a rectangular array of objects arranged in rows and columns.
	Matrices have a compile-time defined size.
	@ingroup core_math
	@tparam ObjectType The type of objects for the matrix to hold
	@tparam Rows The number of rows for the matrix to contain
	@tparam Columns The number of columns for the matrix to contain

	@todo matrix multiplication, division
	@todo static assertion tests
 */
template <typename ObjectType, size_t Rows, size_t Columns>
class LIMES_EXPORT Matrix final
{
	static_assert (Rows > 0);
	static_assert (Columns > 0);

public:

	using Row	 = std::array<ObjectType, Columns>;
	using Column = std::array<ObjectType, Rows>;

	/** You can access the stored type of objects through this typedef. */
	using Element = ObjectType;

	/** Returns the number of rows the %matrix contains. */
	[[nodiscard]] static consteval size_t getNumRows() noexcept { return Rows; }

	/** Returns the number of columns the %matrix contains. */
	[[nodiscard]] static consteval size_t getNumColumns() noexcept { return Columns; }

	/** Returns the number of total number of elements the %matrix contains. */
	[[nodiscard]] static consteval size_t getTotalNumElements() noexcept { return Rows * Columns; }

	/** Returns true if the %matrix is square; ie, its number of rows is equal to its number of columns. */
	[[nodiscard]] static consteval bool isSquare() noexcept { return Rows == Columns; }

	/** Returns a reference to a row of the %matrix. */
	[[nodiscard]] Row& operator[] (size_t row) noexcept;

	/** Returns a reference to a row of the %matrix. */
	[[nodiscard]] const Row& operator[] (size_t row) const noexcept;

	/** Returns a reference to an object at a specified location in the %matrix. */
	[[nodiscard]] ObjectType& get (size_t row, size_t column) noexcept;

	/** Returns a reference to an object at a specified location in the %matrix. */
	[[nodiscard]] const ObjectType& get (size_t row, size_t column) const noexcept;

	/** Assigns an object at a specified location in the %matrix to a new value. */
	void set (size_t row, size_t column, const ObjectType& setTo);

	/** Returns a column of the %matrix.
		@note This function creates copies of each object! To obtain references, use either \c operator[] or \c get() .
	 */
	[[nodiscard]] Column getColumn (size_t columnNumber) const;

	/** Adds each element in \c other to the element at the corresponding position in this %matrix. */
	Matrix& operator+= (const Matrix& other) noexcept;

	/** Subtracts each element in \c other from the element at the corresponding position in this %matrix. */
	Matrix& operator-= (const Matrix& other) noexcept;

	/** Adds the \c object to every object in this %matrix. */
	Matrix& operator+= (const ObjectType& object) noexcept;

	/** Subtracts the \c object from every object in this %matrix. */
	Matrix& operator-= (const ObjectType& object) noexcept;

	/** Returns true if every object in both matrices is equivalent. */
	[[nodiscard]] bool operator== (const Matrix& other) const noexcept;

	/** Returns true if not every object in both matrices is equivalent. */
	[[nodiscard]] bool operator!= (const Matrix& other) const noexcept;

	/** Returns a new %matrix containing copies of a subset of this matrix's objects. */
	[[nodiscard]] auto getSubmatrix (size_t startingRow, size_t startingColumn) const noexcept;

private:

	std::array<Row, Rows> rows;
};


template <typename ObjectType, size_t Rows, size_t Columns>
typename Matrix<ObjectType, Rows, Columns>::Row& Matrix<ObjectType, Rows, Columns>::operator[] (size_t row) noexcept
{
	LIMES_ASSERT (row < Rows);
	return rows[row];
}

template <typename ObjectType, size_t Rows, size_t Columns>
const typename Matrix<ObjectType, Rows, Columns>::Row& Matrix<ObjectType, Rows, Columns>::operator[] (size_t row) const noexcept
{
	LIMES_ASSERT (row < Rows);
	return rows[row];
}

template <typename ObjectType, size_t Rows, size_t Columns>
ObjectType& Matrix<ObjectType, Rows, Columns>::get (size_t row, size_t column) noexcept
{
	LIMES_ASSERT (row < Rows && column < Columns);
	return rows[row][column];
}

template <typename ObjectType, size_t Rows, size_t Columns>
const ObjectType& Matrix<ObjectType, Rows, Columns>::get (size_t row, size_t column) const noexcept
{
	LIMES_ASSERT (row < Rows && column < Columns);
	return rows[row][column];
}

template <typename ObjectType, size_t Rows, size_t Columns>
void Matrix<ObjectType, Rows, Columns>::set (size_t row, size_t column, const ObjectType& setTo)
{
	LIMES_ASSERT (row < Rows && column < Columns);
	rows[row][column] = setTo;
}

template <typename ObjectType, size_t Rows, size_t Columns>
typename Matrix<ObjectType, Rows, Columns>::Column Matrix<ObjectType, Rows, Columns>::getColumn (size_t columnNumber) const
{
	LIMES_ASSERT (columnNumber < Columns);

	Column column;

	for (size_t i = 0; i < Rows; ++i)
		column[i] = rows[i][columnNumber];

	return column;
}

template <typename ObjectType, size_t Rows, size_t Columns>
Matrix<ObjectType, Rows, Columns>& Matrix<ObjectType, Rows, Columns>::operator+= (const Matrix& other) noexcept
{
	for (size_t row = 0; row < Rows; ++row)
		for (size_t column = 0; column < Columns; ++column)
			rows[row][column] += other[row][column];

	return *this;
}

template <typename ObjectType, size_t Rows, size_t Columns>
Matrix<ObjectType, Rows, Columns>& Matrix<ObjectType, Rows, Columns>::operator-= (const Matrix& other) noexcept
{
	for (size_t row = 0; row < Rows; ++row)
		for (size_t column = 0; column < Columns; ++column)
			rows[row][column] -= other[row][column];

	return *this;
}

template <typename ObjectType, size_t Rows, size_t Columns>
Matrix<ObjectType, Rows, Columns>& Matrix<ObjectType, Rows, Columns>::operator+= (const ObjectType& object) noexcept
{
	for (size_t row = 0; row < Rows; ++row)
		for (size_t column = 0; column < Columns; ++column)
			rows[row][column] += object;

	return *this;
}

template <typename ObjectType, size_t Rows, size_t Columns>
Matrix<ObjectType, Rows, Columns>& Matrix<ObjectType, Rows, Columns>::operator-= (const ObjectType& object) noexcept
{
	for (size_t row = 0; row < Rows; ++row)
		for (size_t column = 0; column < Columns; ++column)
			rows[row][column] -= object;

	return *this;
}

template <typename ObjectType, size_t Rows, size_t Columns>
bool Matrix<ObjectType, Rows, Columns>::operator== (const Matrix& other) const noexcept
{
	for (size_t row = 0; row < Rows; ++row)
		for (size_t column = 0; column < Columns; ++column)
			if (rows[row][column] != other[row][column])
				return false;

	return true;
}

template <typename ObjectType, size_t Rows, size_t Columns>
bool Matrix<ObjectType, Rows, Columns>::operator!= (const Matrix& other) const noexcept
{
	return ! (*this == other);
}

template <typename ObjectType, size_t Rows, size_t Columns>
auto Matrix<ObjectType, Rows, Columns>::getSubmatrix (size_t startingRow, size_t startingColumn) const noexcept
{
	const auto NumRows	  = Rows - startingRow;
	const auto NumColumns = Columns - startingColumn;

	Matrix<ObjectType, NumRows, NumColumns> newMatrix;

	for (size_t row = startingRow, newRow = 0; row < startingRow + NumRows; ++row, ++newRow)
		for (size_t column = startingColumn, newColumn = 0; column < startingColumn + NumColumns; ++column, ++newColumn)
			newMatrix[newRow][newColumn] = rows[row][column];

	return newMatrix;
}

}  // namespace math

LIMES_END_NAMESPACE
