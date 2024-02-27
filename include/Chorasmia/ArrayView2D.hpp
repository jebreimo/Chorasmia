//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <vector>
#include "ChorasmiaException.hpp"
#include "ArrayView2DIterator.hpp"

namespace Chorasmia
{
    template <typename It1, typename It2>
    bool equal_sequences_with_gaps(It1 seq1, It1 endSeq1, size_t gapSize1,
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
                              size_t row_gap_size) noexcept
            : data_(data),
              row_count_(rows),
              col_count_(columns),
              row_gap_(row_gap_size)
        {}

        [[nodiscard]]
        const T& operator()(size_t row, size_t column) const noexcept
        {
            return data_[row * row_size() + column];
        }

        [[nodiscard]]
        const T& operator()(std::pair<size_t, size_t> pos) const noexcept
        {
            return operator()(pos.first, pos.second);
        }

        [[nodiscard]]
        ArrayView<T> operator[](size_t r) const noexcept
        {
            return {data_ + r * row_size(), col_count()};
        }

        [[nodiscard]]
        constexpr const T* data() const noexcept
        {
            return data_;
        }

        [[nodiscard]]
        constexpr bool empty() const noexcept
        {
            return row_count_ == 0 || col_count_ == 0;
        }

        [[nodiscard]]
        constexpr bool contiguous() const noexcept
        {
            return row_gap_ == 0 || row_count_ <= 1;
        }

        [[nodiscard]]
        ArrayView<T> array() const
        {
            if (!contiguous())
                CHORASMIA_THROW("Can not create ArrayView from non-contiguous ArrayView2D.");
            return ArrayView<T>(data_, value_count());
        }

        [[nodiscard]]
        ArrayView2D<T> subarray(size_t row, size_t column,
                                size_t n_rows = SIZE_MAX,
                                size_t n_cols = SIZE_MAX) const
        {
            row = std::min(row, row_count());
            column = std::min(column, col_count());
            n_rows = std::min(n_rows, row_count() - row);
            n_cols = std::min(n_cols, col_count() - column);
            return {data() + row * row_size() + column,
                    n_rows,
                    n_cols,
                    row_gap_ + col_count() - n_cols};
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> dimensions() const noexcept
        {
            return {row_count_, col_count_};
        }

        [[nodiscard]]
        constexpr size_t row_count() const noexcept
        {
            return row_count_;
        }

        [[nodiscard]]
        constexpr size_t col_count() const noexcept
        {
            return col_count_;
        }

        [[nodiscard]]
        constexpr size_t value_count() const noexcept
        {
            return row_count_ * col_count_;
        }

        [[nodiscard]]
        ConstIterator begin() const noexcept
        {
            return ConstIterator({data_, col_count()}, row_gap_);
        }

        [[nodiscard]]
        ConstIterator end() const noexcept
        {
            return ConstIterator({data_ + row_count() * row_size(), col_count()},
                                 row_gap_);
        }

        friend bool operator==(const ArrayView2D& a, const ArrayView2D& b)
        {
            if (a.row_count() != b.row_count()
                || a.col_count() != b.col_count())
                return false;
            if (a.row_gap_ == b.row_gap_ && a.data() == b.data())
                return true;
            return equal_sequences_with_gaps(
                a.data(), a.data() + a.row_size(), a.row_gap_,
                b.data(), b.row_gap_,
                a.col_count());
        }

        [[nodiscard]]
        friend bool
        operator!=(const ArrayView2D& a, const ArrayView2D& b)
        {
            return !(a == b);
        }
    private:
        [[nodiscard]]
        constexpr size_t row_size() const
        {
            return col_count() + row_gap_;
        }

        const T* data_ = nullptr;
        size_t row_count_ = 0;
        size_t col_count_ = 0;
        size_t row_gap_ = 0;
    };
}
