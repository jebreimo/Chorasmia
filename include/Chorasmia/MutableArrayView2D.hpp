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
                                     Size2D<size_t> size) noexcept
            : MutableArrayView2D(data, size, 0)
        {}

        constexpr MutableArrayView2D(T* data,
                                     Size2D<size_t> size,
                                     size_t row_gap_size) noexcept
            : data_(data),
              size_(size),
              row_gap_(row_gap_size)
        {}

        [[nodiscard]]
        T& operator[](const Index2D<size_t>& pos) const noexcept
        {
            return data_[pos.row * row_size() + pos.column];
        }

        [[nodiscard]]
        constexpr MutableArrayView<T> row(size_t row) const noexcept
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
            return is_empty(size_);
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
            {
                CHORASMIA_THROW(
                    "Can not create MutableArrayView from non-contiguous MutableArrayView2D.");
            }
            return MutableArrayView<T>(data(), value_count());
        }

        [[nodiscard]]
        MutableArrayView2D subarray(Extent2D<size_t> extent) const
        {
            extent = clamp(extent, size_);
            return {
                data() + extent.origin.row * col_count() + extent.origin.column,
                extent.size,
                col_count() - extent.size.columns
            };
        }

        [[nodiscard]]
        constexpr Index2D<size_t> dimensions() const noexcept
        {
            return {size_.rows, size_.columns};
        }

        [[nodiscard]]
        constexpr size_t row_count() const noexcept
        {
            return size_.rows;
        }

        [[nodiscard]]
        constexpr size_t col_count() const noexcept
        {
            return size_.columns;
        }

        [[nodiscard]]
        constexpr size_t value_count() const noexcept
        {
            return size_.rows * size_.columns;
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
            if (a.size_ != b.size_)
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
        Index2D<size_t> size_;
        size_t row_gap_ = 0;
    };
}
