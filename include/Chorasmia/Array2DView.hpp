//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "RowIterator.hpp"
#include "ChorasmiaException.hpp"

namespace GridLib
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
    class Array2DView
    {
    public:
        constexpr Array2DView() = default;

        constexpr Array2DView(const T* data,
                              size_t rows,
                              size_t columns) noexcept
            : Array2DView(data, rows, columns, 0)
        {}

        constexpr Array2DView(const T* data,
                              size_t rows,
                              size_t columns,
                              size_t rowGapSize) noexcept
            : m_Data(data),
              m_Size(rows, columns),
              m_RowGap(rowGapSize)
        {}

        const T& operator()(size_t row, size_t column) const
        {
            return m_Data[row * rowSize() + column];
        }

        constexpr const T* data() const
        {
            return m_Data;
        }

        [[nodiscard]]
        constexpr bool empty() const
        {
            return m_Size == std::pair(size_t(0), size_t(0));
        }

        [[nodiscard]]
        constexpr bool contiguous() const
        {
            return m_RowGap == 0;
        }

        [[nodiscard]]
        ArrayView<T> array() const
        {
            if (!contiguous())
                CHORASMIA_THROW("Can not create ArrayView from non-contiguous Array2DView.");
            return ArrayView<T>(m_Data, valueCount());
        }

        [[nodiscard]]
        Array2DView<T> subarray(size_t row, size_t column,
                                size_t nrows = SIZE_MAX,
                                size_t ncolumns = SIZE_MAX)
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
        ArrayView<T> row(size_t r) const
        {
            return {m_Data + r * rowSize(), columnCount()};
        }

        [[nodiscard]]
        constexpr size_t rowCount() const
        {
            return m_Size.first;
        }

        [[nodiscard]]
        constexpr size_t columnCount() const
        {
            return m_Size.second;
        }

        [[nodiscard]]
        constexpr size_t valueCount() const
        {
            return m_Size.second;
        }

        [[nodiscard]]
        RowIterator<T> begin() const
        {
            return RowIterator<T>({m_Data, columnCount()}, m_RowGap);
        }

        [[nodiscard]]
        RowIterator<T> end() const
        {
            return RowIterator<T>({m_Data + rowCount() * rowSize(),
                                   m_Size.second}, m_RowGap);
        }

        friend bool operator==(const Array2DView& a, const Array2DView& b)
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
    private:
        [[nodiscard]]
        constexpr size_t rowSize() const
        {
            return columnCount() + m_RowGap;
        }

        const T* m_Data = nullptr;
        std::pair<size_t, size_t> m_Size;
        size_t m_RowGap = 0;
    };

    template <typename T>
    bool operator!=(const Array2DView<T>& a, const Array2DView<T>& b)
    {
        return !(a == b);
    }
}
