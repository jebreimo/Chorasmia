//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-01-04.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "ArrayView2D.hpp"

namespace Chorasmia
{
    template <typename T>
    class MutableArrayView2D
    {
    public:
        using MutableIterator = ArrayView2DIterator<T, true>;
        using ConstIterator = ArrayView2DIterator<T>;

        constexpr MutableArrayView2D() = default;

        constexpr MutableArrayView2D(T* data,
                                     size_t rows,
                                     size_t columns) noexcept
            : MutableArrayView2D(data, rows, columns, 0)
        {}

        constexpr MutableArrayView2D(T* data,
                                     size_t rows,
                                     size_t columns,
                                     size_t rowGapSize) noexcept
            : m_Data(data),
              m_RowCount(rows),
              m_ColumnCount(columns),
              m_RowGap(rowGapSize)
        {}

        [[nodiscard]]
        T& operator()(size_t row, size_t column) const noexcept
        {
            return m_Data[row * rowSize() + column];
        }

        [[nodiscard]]
        T& operator()(std::pair<size_t, size_t> pos) const noexcept
        {
            return operator()(pos.first, pos.second);
        }

        [[nodiscard]]
        constexpr MutableArrayView<T> operator[](size_t row) const noexcept
        {
            return {data() + row * rowSize(), columnCount()};
        }

        [[nodiscard]]
        constexpr T* data() const noexcept
        {
            return m_Data;
        }

        [[nodiscard]]
        constexpr bool empty() const noexcept
        {
            return m_RowCount == 0 || m_ColumnCount == 0;
        }

        [[nodiscard]]
        constexpr bool contiguous() const noexcept
        {
            return m_RowGap == 0;
        }

        constexpr ArrayView2D<T> view() const noexcept
        {
            return {data(), rowCount(), columnCount(), m_RowGap};
        }

        [[nodiscard]]
        MutableArrayView<T> array() const
        {
            if (!contiguous())
                CHORASMIA_THROW("Can not create MutableArrayView from non-contiguous MutableArrayView2D.");
            return MutableArrayView<T>(data(), valueCount());
        }

        [[nodiscard]]
        MutableArrayView2D<T> subarray(size_t row, size_t column,
                                       size_t n_rows = SIZE_MAX,
                                       size_t n_cols = SIZE_MAX) const
        {
            row = std::min(row, rowCount());
            column = std::min(column, columnCount());
            n_rows = std::min(n_rows, rowCount() - row);
            n_cols = std::min(n_cols, columnCount() - column);
            return {data() + row * rowSize() + column,
                    n_rows,
                    n_cols,
                    m_RowGap + columnCount() - n_cols};
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> dimensions() const noexcept
        {
            return {m_RowCount, m_ColumnCount};
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

        [[nodiscard]]
        MutableIterator begin() const noexcept
        {
            return MutableIterator({m_Data, columnCount()}, m_RowGap);
        }

        [[nodiscard]]
        MutableIterator end() const noexcept
        {
            return MutableIterator({m_Data + rowCount() * rowSize(), columnCount()},
                                   m_RowGap);
        }

        [[nodiscard]]
        friend bool
        operator==(const MutableArrayView2D& a, const MutableArrayView2D& b)
        {
            if (a.rowCount() != b.rowCount()
                || a.columnCount() != b.columnCount())
                return false;
            if (a.m_RowGap == b.m_RowGap && a.data() == b.data())
                return true;
            return equalSequencesWithGaps(
                a.data(), a.data() + a.rowCount() * a.rowSize(), a.m_RowGap,
                b.data(), b.m_RowGap,
                a.columnCount());
        }

        [[nodiscard]]
        friend bool
        operator!=(const MutableArrayView2D& a, const MutableArrayView2D& b)
        {
            return !(a == b);
        }
    private:
        [[nodiscard]]
        constexpr size_t rowSize() const
        {
            return columnCount() + m_RowGap;
        }

        T* m_Data = nullptr;
        size_t m_RowCount = 0;
        size_t m_ColumnCount = 0;
        size_t m_RowGap = 0;
    };
}
