//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-01-04.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "MutableArrayView2D.hpp"

namespace Chorasmia
{
    template <typename T>
    class Array2D
    {
    public:
        using MutableIterator = ArrayView2DIterator<T, true>;
        using ConstIterator = ArrayView2DIterator<T>;

        Array2D() = default;

        Array2D(size_t rows, size_t columns)
            : m_Buffer(rows * columns),
              m_RowCount(rows),
              m_ColumnCount(columns)
        {}

        Array2D(const T* values, size_t rows, size_t columns)
            : m_Buffer(values, values + rows * columns),
              m_RowCount(rows),
              m_ColumnCount(columns)
        {}

        Array2D(std::vector<T> values, size_t rows, size_t columns)
            : m_Buffer(std::move(values)),
              m_RowCount(rows),
              m_ColumnCount(columns)
        {
            if (valueCount() != m_Buffer.size())
                throw std::runtime_error("Array2D has incorrect size.");
        }

        constexpr operator ArrayView2D<T>() const noexcept
        {
            return {data(), rowCount(), columnCount()};
        }

        constexpr operator MutableArrayView2D<T>() noexcept
        {
            return {data(), rowCount(), columnCount()};
        }

        [[nodiscard]]
        const T& operator()(size_t row, size_t column) const noexcept
        {
            return m_Buffer[row * columnCount() + column];
        }

        [[nodiscard]]
        T& operator()(size_t row, size_t column) noexcept
        {
            return m_Buffer[row * columnCount() + column];
        }

        [[nodiscard]]
        constexpr ArrayView<T> operator[](size_t row) const
        {
            return {data() + row * columnCount(), columnCount()};
        }

        [[nodiscard]]
        constexpr MutableArrayView<T> operator[](size_t row)
        {
            return {data() + row * columnCount(), columnCount()};
        }

        [[nodiscard]]
        const T* data() const noexcept
        {
            return m_Buffer.data();
        }

        [[nodiscard]]
        T* data() noexcept
        {
            return m_Buffer.data();
        }

        [[nodiscard]]
        bool empty() const noexcept
        {
            return m_Buffer.empty();
        }

        [[nodiscard]]
        size_t size() const noexcept
        {
            return m_Buffer.size();
        }

        [[nodiscard]]
        ArrayView<T> array() const
        {
            return ArrayView<T>(data(), valueCount());
        }

        [[nodiscard]]
        MutableArrayView<T> array()
        {
            return MutableArrayView<T>(data(), valueCount());
        }

        [[nodiscard]]
        ArrayView2D<T> subarray(size_t row, size_t column,
                                size_t nrows = SIZE_MAX,
                                size_t ncolumns = SIZE_MAX) const
        {
            row = std::min(row, rowCount());
            column = std::min(column, columnCount());
            nrows = std::min(nrows, rowCount() - row);
            ncolumns = std::min(ncolumns, columnCount() - column);
            return {data() + row * columnCount() + column,
                    nrows,
                    ncolumns,
                    columnCount() - ncolumns};
        }

        [[nodiscard]]
        MutableArrayView2D<T> subarray(size_t row, size_t column,
                                       size_t nrows = SIZE_MAX,
                                       size_t ncolumns = SIZE_MAX)
        {
            row = std::min(row, rowCount());
            column = std::min(column, columnCount());
            nrows = std::min(nrows, rowCount() - row);
            ncolumns = std::min(ncolumns, columnCount() - column);
            return {data() + row * columnCount() + column,
                    nrows,
                    ncolumns,
                    columnCount() - ncolumns};
        }

        [[nodiscard]]
        constexpr size_t rowCount() const noexcept
        {
            return m_RowCount;
        }

        [[nodiscard]]
        constexpr size_t columnCount() const noexcept
        {
            return m_ColumnCount;
        }

        [[nodiscard]]
        constexpr size_t valueCount() const noexcept
        {
            return m_RowCount * m_ColumnCount;
        }

        void resize(size_t rows, size_t columns)
        {
            auto oldValueCount = valueCount();
            auto oldRows = this->rowCount();
            auto oldColumns = this->columnCount();
            auto newSize = rows * columns;
            m_Buffer.resize(newSize);
            m_RowCount = rows;
            m_ColumnCount = columns;

            if (oldRows == 0)
                return;

            if (columns > oldColumns)
            {
                auto src = m_Buffer.data() + oldValueCount - 1;
                auto dst = m_Buffer.data() + (oldRows - 1) * columns + oldColumns - 1;
                while (src != dst)
                {
                    for (size_t i = 0; i < oldColumns; ++i)
                        *dst-- = *src--;
                    for (size_t i = 0; i < columns - oldColumns; ++i)
                        *dst-- = 0;
                }
            }
            else if (columns < oldColumns)
            {
                auto src = m_Buffer.data() + oldColumns;
                auto dst = m_Buffer.data() + columns;
                for (size_t i = 0; i < oldRows - 1; ++i)
                {
                    for (size_t j = 0; j < columns; ++j)
                        *dst++ = *src++;
                    src += oldColumns - columns;
                }
            }
        }

        [[nodiscard]]
        MutableIterator begin() noexcept
        {
            return MutableIterator({data(), columnCount()});
        }

        [[nodiscard]]
        ConstIterator begin() const noexcept
        {
            return ConstIterator({data(), columnCount()});
        }

        [[nodiscard]]
        MutableIterator end() noexcept
        {
            return MutableIterator({data() + valueCount(), columnCount()});
        }

        [[nodiscard]]
        ConstIterator end() const noexcept
        {
            return ConstIterator({data() + valueCount(), columnCount()});
        }

        [[nodiscard]]
        std::vector<T> release()
        {
            m_RowCount = m_ColumnCount = 0;
            auto tmp = std::move(m_Buffer);
            return std::move(tmp);
        }

        void fill(const T& value)
        {
            std::fill(m_Buffer.begin(), m_Buffer.end(), value);
        }

        [[nodiscard]]
        friend bool operator==(const Array2D& a, const Array2D& b)
        {
            return a.rowCount() == b.rowCount()
                   && a.columnCount() == b.columnCount()
                   && a.m_Buffer == b.m_Buffer;
        }

        [[nodiscard]]
        friend bool operator!=(const Array2D& a, const Array2D& b)
        {
            return !(a == b);
        }

        [[nodiscard]]
        friend const T* data(const Array2D& a)
        {
            return a.data();
        }

        [[nodiscard]]
        friend T* data(Array2D& a)
        {
            return a.data();
        }

        [[nodiscard]]
        friend size_t size(const Array2D& a)
        {
            return a.size();
        }
    private:
        std::vector<T> m_Buffer;
        size_t m_RowCount = 0;
        size_t m_ColumnCount = 0;
    };
}
