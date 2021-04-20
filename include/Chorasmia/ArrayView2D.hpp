//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "ChorasmiaException.hpp"
#include "ArrayView2DIterator.hpp"

namespace Chorasmia
{
    template <typename It1, typename It2>
    bool equalSequencesWithGaps(It1 seq1, It1 endSeq1, size_t gapSize1,
                                It2 seq2, size_t gapSize2,
                                size_t seqSize)
    {
        if (gapSize1 == 0 && gapSize2 == 0)
            return std::equal(seq1, endSeq1, seq2);

        while (seq1 != endSeq1)
        {
            if (!std::equal(seq1, seq1 + seqSize, seq2))
                return false;
            seq1 += seqSize;
            if (seq1 == endSeq1)
                break;
            seq1 += gapSize1;
            seq2 += seqSize + gapSize2;
        }
        return true;
    }

    template <typename T>
    class ArrayView2D
    {
    public:
        using ConstIterator = ArrayView2DIterator<T>;

        constexpr ArrayView2D() = default;

        constexpr ArrayView2D(const T* data,
                              size_t rows,
                              size_t columns) noexcept
            : ArrayView2D(data, rows, columns, 0)
        {}

        constexpr ArrayView2D(const T* data,
                              size_t rows,
                              size_t columns,
                              size_t rowGapSize) noexcept
            : m_Data(data),
              m_RowCount(rows),
              m_ColumnCount(columns),
              m_RowGap(rowGapSize)
        {}

        [[nodiscard]]
        const T& operator()(size_t row, size_t column) const noexcept
        {
            return m_Data[row * rowSize() + column];
        }

        [[nodiscard]]
        const T& operator()(std::pair<size_t, size_t> pos) const noexcept
        {
            return operator()(pos.first, pos.second);
        }

        [[nodiscard]]
        ArrayView<T> operator[](size_t r) const noexcept
        {
            return {m_Data + r * rowSize(), columnCount()};
        }

        [[nodiscard]]
        constexpr const T* data() const noexcept
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
            return m_RowGap == 0 || m_RowCount <= 1;
        }

        [[nodiscard]]
        ArrayView<T> array() const
        {
            if (!contiguous())
                CHORASMIA_THROW("Can not create ArrayView from non-contiguous ArrayView2D.");
            return ArrayView<T>(m_Data, valueCount());
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
            return {data() + row * rowSize() + column,
                    nrows,
                    ncolumns,
                    m_RowGap + columnCount() - ncolumns};
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
        ConstIterator begin() const noexcept
        {
            return ConstIterator({m_Data, columnCount()}, m_RowGap);
        }

        [[nodiscard]]
        ConstIterator end() const noexcept
        {
            return ConstIterator({m_Data + rowCount() * rowSize(), columnCount()},
                                 m_RowGap);
        }

        friend bool operator==(const ArrayView2D& a, const ArrayView2D& b)
        {
            if (a.rowCount() != b.rowCount()
                || a.columnCount() != b.columnCount())
                return false;
            if (a.m_RowGap == b.m_RowGap && a.data() == b.data())
                return true;
            return equalSequencesWithGaps(
                a.data(), a.data() + a.rowSize(), a.m_RowGap,
                b.data(), b.m_RowGap,
                a.columnCount());
        }

        [[nodiscard]]
        friend bool
        operator!=(const ArrayView2D& a, const ArrayView2D& b)
        {
            return !(a == b);
        }
    private:
        [[nodiscard]]
        constexpr size_t rowSize() const
        {
            return columnCount() + m_RowGap;
        }

        const T* m_Data = nullptr;
        size_t m_RowCount = 0;
        size_t m_ColumnCount = 0;
        size_t m_RowGap = 0;
    };
}
