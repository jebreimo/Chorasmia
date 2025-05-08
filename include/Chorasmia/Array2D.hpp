//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-01-04.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "MutableArrayView2D.hpp"
#include <string>

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
            : buffer_(rows * columns),
              row_count_(rows),
              col_count_(columns)
        {}

        Array2D(const T* values, size_t rows, size_t columns)
            : buffer_(values, values + rows * columns),
              row_count_(rows),
              col_count_(columns)
        {}

        Array2D(std::vector<T> values, size_t rows, size_t columns)
            : buffer_(std::move(values)),
              row_count_(rows),
              col_count_(columns)
        {
            if (value_count() != buffer_.size())
            {
                CHORASMIA_THROW("Array2D has incorrect size. Expected "
                    + std::to_string(value_count()) +
                    " but got " + std::to_string(buffer_.size()));
            }
        }

        [[nodiscard]]
        const T& operator()(size_t row, size_t column) const noexcept
        {
            return buffer_[row * col_count() + column];
        }

        [[nodiscard]]
        T& operator()(size_t row, size_t column) noexcept
        {
            return buffer_[row * col_count() + column];
        }

        [[nodiscard]]
        const T& operator()(const std::pair<size_t, size_t>& pos) const noexcept
        {
            return operator()(pos.first, pos.second);
        }

        [[nodiscard]]
        T& operator()(const std::pair<size_t, size_t>& pos) noexcept
        {
            return operator()(pos.first, pos.second);
        }

        [[nodiscard]]
        constexpr ArrayView<T> operator[](size_t row) const
        {
            return {data() + row * col_count(), col_count()};
        }

        [[nodiscard]]
        constexpr MutableArrayView<T> operator[](size_t row)
        {
            return {data() + row * col_count(), col_count()};
        }

        [[nodiscard]]
        const T* data() const noexcept
        {
            return buffer_.data();
        }

        [[nodiscard]]
        T* data() noexcept
        {
            return buffer_.data();
        }

        [[nodiscard]]
        bool empty() const noexcept
        {
            return buffer_.empty();
        }

        [[nodiscard]]
        size_t size() const noexcept
        {
            return buffer_.size();
        }

        constexpr ArrayView2D<T> view() const noexcept
        {
            return {data(), row_count(), col_count()};
        }

        constexpr MutableArrayView2D<T> mut() noexcept
        {
            return {data(), row_count(), col_count()};
        }

        [[nodiscard]]
        ArrayView<T> array() const
        {
            return ArrayView<T>(data(), value_count());
        }

        [[nodiscard]]
        MutableArrayView<T> array()
        {
            return MutableArrayView<T>(data(), value_count());
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
            return {data() + row * col_count() + column,
                    n_rows,
                    n_cols,
                    col_count() - n_cols};
        }

        [[nodiscard]]
        MutableArrayView2D<T> subarray(size_t row, size_t column,
                                       size_t n_rows = SIZE_MAX,
                                       size_t n_cols = SIZE_MAX)
        {
            row = std::min(row, row_count());
            column = std::min(column, col_count());
            n_rows = std::min(n_rows, row_count() - row);
            n_cols = std::min(n_cols, col_count() - column);
            return {data() + row * col_count() + column,
                    n_rows,
                    n_cols,
                    col_count() - n_cols};
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

        void resize(size_t rows, size_t columns)
        {
            auto old_value_count = value_count();
            auto old_rows = this->row_count();
            auto old_columns = this->col_count();
            auto new_size = rows * columns;
            buffer_.resize(new_size);
            row_count_ = rows;
            col_count_ = columns;

            if (old_rows == 0)
                return;

            if (columns > old_columns)
            {
                auto src = buffer_.data() + old_value_count - 1;
                auto dst = buffer_.data() + (old_rows - 1) * columns + old_columns - 1;
                while (src != dst)
                {
                    for (size_t i = 0; i < old_columns; ++i)
                        *dst-- = *src--;
                    for (size_t i = 0; i < columns - old_columns; ++i)
                        *dst-- = 0;
                }
            }
            else if (columns < old_columns)
            {
                auto src = buffer_.data() + old_columns;
                auto dst = buffer_.data() + columns;
                for (size_t i = 0; i < old_rows - 1; ++i)
                {
                    for (size_t j = 0; j < columns; ++j)
                        *dst++ = *src++;
                    src += old_columns - columns;
                }
            }
        }

        [[nodiscard]]
        MutableIterator begin() noexcept
        {
            return MutableIterator({data(), col_count()});
        }

        [[nodiscard]]
        ConstIterator begin() const noexcept
        {
            return ConstIterator({data(), col_count()});
        }

        [[nodiscard]]
        MutableIterator end() noexcept
        {
            return MutableIterator({data() + value_count(), col_count()});
        }

        [[nodiscard]]
        ConstIterator end() const noexcept
        {
            return ConstIterator({data() + value_count(), col_count()});
        }

        [[nodiscard]]
        std::vector<T> release()
        {
            row_count_ = col_count_ = 0;
            auto tmp = std::move(buffer_);
            return std::move(tmp);
        }

        void fill(const T& value)
        {
            std::fill(buffer_.begin(), buffer_.end(), value);
        }

        [[nodiscard]]
        friend bool operator==(const Array2D& a, const Array2D& b)
        {
            return a.row_count() == b.row_count()
                   && a.col_count() == b.col_count()
                   && a.buffer_ == b.buffer_;
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
        std::vector<T> buffer_;
        size_t row_count_ = 0;
        size_t col_count_ = 0;
    };
}
