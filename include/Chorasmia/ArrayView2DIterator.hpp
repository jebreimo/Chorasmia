//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <type_traits>
#include "ArrayView.hpp"
#include "MutableArrayView.hpp"

namespace Chorasmia
{
    template <typename T, bool IsMutable = false>
    class ArrayView2DIterator
    {
    public:
        using ArrayViewType = std::conditional_t<IsMutable,
                                                 MutableArrayView<T>,
                                                 ArrayView<T>>;

        ArrayView2DIterator() = default;

        constexpr explicit ArrayView2DIterator(const ArrayViewType& row) noexcept
            : row_(row)
        {}

        constexpr ArrayView2DIterator(const ArrayViewType& row,
                                      size_t row_gap) noexcept
            : row_(row),
              row_gap_(row_gap)
        {}

        ArrayView2DIterator& operator++()
        {
            row_ = ArrayViewType{
                row_.data() + row_size(),
                row_.size()
            };
            return *this;
        }

        ArrayView2DIterator operator++(int)
        {
            auto p = *this;
            row_ = ArrayViewType{
                row_.data() + row_size(),
                row_.size()
            };
            return p;
        }

        ArrayView2DIterator& operator+=(size_t n)
        {
            row_ = ArrayViewType{
                row_.data() + n * row_size(),
                row_.size()
            };
            return *this;
        }

        ArrayView2DIterator& operator--()
        {
            row_ = {row_.data() - row_size(), row_.size()};
            return *this;
        }

        ArrayView2DIterator operator--(int)
        {
            auto p = *this;
            row_ = ArrayViewType{row_.data() - row_size(), row_.size()};
            return p;
        }

        ArrayView2DIterator& operator-=(size_t n)
        {
            row_ = ArrayViewType{row_.data() - n * row_size(), row_.size()};
            return *this;
        }

        ArrayViewType operator[](size_t i) const
        {
            return ArrayViewType{row_.data() + i * row_size(), row_.size()};
        }

        const ArrayViewType& operator*() const
        {
            return row_;
        }

        const ArrayViewType* operator->() const
        {
            return &row_;
        }

        [[nodiscard]]
        friend bool operator==(const ArrayView2DIterator& a, const ArrayView2DIterator& b)
        {
            return are_identical(a.row_, b.row_) && a.row_gap_ == b.row_gap_;
        }

        [[nodiscard]]
        friend bool operator!=(const ArrayView2DIterator& a, const ArrayView2DIterator& b)
        {
            return !(a == b);
        }

    private:
        [[nodiscard]]
        constexpr size_t row_size() const
        {
            return row_.size() + row_gap_;
        }

        ArrayViewType row_;
        size_t row_gap_ = 0;
    };
}
