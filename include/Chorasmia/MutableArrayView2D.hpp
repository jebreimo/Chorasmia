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
                                     size_t row_gap_size) noexcept
            : data_(data),
              row_count_(rows),
              col_count_(columns),
              row_gap_(row_gap_size)
        {}

        [[nodiscard]]
        T& operator()(size_t row, size_t column) const noexcept
        {
            return data_[row * row_size() + column];
        }

        [[nodiscard]]
        T& operator()(std::pair<size_t, size_t> pos) const noexcept
        {
            return operator()(pos.first, pos.second);
        }

        [[nodiscard]]
        constexpr MutableArrayView<T> operator[](size_t row) const noexcept
        {
            return {data() + row * row_size(), col_count()};
        }

        [[nodiscard]]
        constexpr T* data() const noexcept
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
            return row_gap_ == 0;
        }

        constexpr ArrayView2D<T> view() const noexcept
        {
            return {data(), row_count(), col_count(), row_gap_};
        }

        [[nodiscard]]
        MutableArrayView<T> array() const
        {
            if (!contiguous())
                CHORASMIA_THROW("Can not create MutableArrayView from non-contiguous MutableArrayView2D.");
            return MutableArrayView<T>(data(), value_count());
        }

        [[nodiscard]]
        MutableArrayView2D<T> subarray(size_t row, size_t column,
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
        MutableIterator begin() const noexcept
        {
            return MutableIterator({data_, col_count()}, row_gap_);
        }

        [[nodiscard]]
        MutableIterator end() const noexcept
        {
            return MutableIterator({data_ + row_count() * row_size(), col_count()},
                                   row_gap_);
        }

        [[nodiscard]]
        friend bool
        operator==(const MutableArrayView2D& a, const MutableArrayView2D& b)
        {
            if (a.row_count() != b.row_count()
                || a.col_count() != b.col_count())
                return false;
            if (a.row_gap_ == b.row_gap_ && a.data() == b.data())
                return true;
            return equal_sequences_with_gaps(
                a.data(), a.data() + a.row_count() * a.row_size(), a.row_gap_,
                b.data(), b.row_gap_,
                a.col_count());
        }

        [[nodiscard]]
        friend bool
        operator!=(const MutableArrayView2D& a, const MutableArrayView2D& b)
        {
            return !(a == b);
        }
    private:
        [[nodiscard]]
        constexpr size_t row_size() const
        {
            return col_count() + row_gap_;
        }

        T* data_ = nullptr;
        size_t row_count_ = 0;
        size_t col_count_ = 0;
        size_t row_gap_ = 0;
    };
}
