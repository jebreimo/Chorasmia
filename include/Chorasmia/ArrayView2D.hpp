//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include "ChorasmiaException.hpp"
#include "ArrayView2DIterator.hpp"
#include "Extent2D.hpp"
#include "Index2D.hpp"

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
                              Size2D<size_t> size) noexcept
            : ArrayView2D(data, size, 0)
        {}

        constexpr ArrayView2D(const T* data,
                              Size2D<size_t> size,
                              size_t row_gap_size) noexcept
            : data_(data),
              size_(size),
              row_gap_(row_gap_size)
        {}

        [[nodiscard]]
        const T& operator[](Index2D<size_t> index) const noexcept
        {
            return data_[index.row * row_size() + index.column];
        }

        [[nodiscard]]
        ArrayView<T> row(size_t r) const noexcept
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
            return is_empty(size_);
        }

        [[nodiscard]]
        constexpr bool contiguous() const noexcept
        {
            return row_gap_ == 0 || size_.rows <= 1;
        }

        [[nodiscard]]
        ArrayView<T> array() const
        {
            if (!contiguous())
                CHORASMIA_THROW("Can not create ArrayView from non-contiguous ArrayView2D.");
            return ArrayView<T>(data_, value_count());
        }

        [[nodiscard]]
        ArrayView2D subarray(Extent2D<size_t> extent) const
        {
            extent = clamp(extent, size_);
            return {
                data() + extent.origin.row * col_count() + extent.origin.column,
                extent.size,
                col_count() - extent.size.columns
            };
        }

        [[nodiscard]]
        constexpr Size2D<size_t> dimensions() const noexcept
        {
            return size_;
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
            if (a.size_ != b.size_)
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
        Size2D<size_t> size_;
        size_t row_gap_ = 0;
    };
}
