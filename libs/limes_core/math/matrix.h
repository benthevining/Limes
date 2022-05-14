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

#include <limes_namespace.h>
#include <limes_export.h>
#include "../system/limes_assert.h"
#include <array>
#include <functional>

LIMES_BEGIN_NAMESPACE

namespace math
{

template <typename ObjectType, size_t Rows, size_t Columns>
class LIMES_EXPORT Matrix final
{
	static_assert (Rows > 0);
	static_assert (Columns > 0);

public:

	using Row	  = std::array<ObjectType, Columns>;
	using Column  = std::array<ObjectType, Rows>;
	using Element = ObjectType;

	[[nodiscard]] static consteval size_t getNumRows() noexcept { return Rows; }
	[[nodiscard]] static consteval size_t getNumColumns() noexcept { return Columns; }
	[[nodiscard]] static consteval size_t getTotalNumElements() noexcept { return Rows * Columns; }

	[[nodiscard]] static consteval bool isSquare() noexcept { return Rows == Columns; }

	[[nodiscard]] Row& operator[] (size_t row) noexcept;

	[[nodiscard]] const Row& operator[] (size_t row) const noexcept;

	[[nodiscard]] ObjectType&		get (size_t row, size_t column) noexcept;
	[[nodiscard]] const ObjectType& get (size_t row, size_t column) const noexcept;

	void set (size_t row, size_t column, const ObjectType& setTo);

	[[nodiscard]] Column getColumn (size_t columnNumber) const;

	Matrix& operator+= (const Matrix& other) noexcept;
	Matrix& operator-= (const Matrix& other) noexcept;

	Matrix& operator+= (const ObjectType& object) noexcept;
	Matrix& operator-= (const ObjectType& object) noexcept;

	[[nodiscard]] bool operator== (const Matrix& other) const noexcept;
	[[nodiscard]] bool operator!= (const Matrix& other) const noexcept;

	template <size_t NumRows, size_t NumColumns>
	[[nodiscard]] Matrix<ObjectType, NumRows, NumColumns> getSubmatrix (size_t startingRow, size_t startingColumn) const noexcept;

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
template <size_t NumRows, size_t NumColumns>
Matrix<ObjectType, NumRows, NumColumns> Matrix<ObjectType, Rows, Columns>::getSubmatrix (size_t startingRow, size_t startingColumn) const noexcept
{
	LIMES_ASSERT (startingRow + NumRows < Rows);
	LIMES_ASSERT (startingColumn + NumColumns < Columns);

	Matrix<ObjectType, NumRows, NumColumns> newMatrix;

	for (size_t row = startingRow, newRow = 0; row < startingRow + NumRows; ++row, ++newRow)
		for (size_t column = startingColumn, newColumn = 0; column < startingColumn + NumColumns; ++column, ++newColumn)
			newMatrix[newRow][newColumn] = rows[row][column];

	return newMatrix;
}

}  // namespace math

LIMES_END_NAMESPACE
