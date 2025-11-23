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

#include "Extent2D.hpp"

namespace Chorasmia
{
    template <typename T>
    class Array2D
    {
    public:
        using MutableIterator = ArrayView2DIterator<T, true>;
        using ConstIterator = ArrayView2DIterator<T>;

        Array2D() = default;

        explicit Array2D(Size2D<size_t> size)
            : buffer_(size.rows * size.columns),
              size_(size)
        {}

        Array2D(const T* values, Size2D<size_t> size)
            : buffer_(values, values + size.rows * size.columns),
              size_(size)
        {}

        Array2D(std::vector<T> values, Size2D<size_t> size)
            : buffer_(std::move(values)),
              size_(size)
        {
            if (value_count() != buffer_.size())
            {
                CHORASMIA_THROW("Array2D has incorrect size. Expected "
                    + std::to_string(value_count()) +
                    " but got " + std::to_string(buffer_.size()));
            }
        }

        [[nodiscard]]
        const T& operator[](Index2D<size_t> index) const noexcept
        {
            return buffer_[index.row * col_count() + index.column];
        }

        [[nodiscard]]
        constexpr ArrayView<T> row(size_t row) const
        {
            return {data() + row * col_count(), col_count()};
        }

        [[nodiscard]]
        constexpr MutableArrayView<T> row(size_t row)
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
            return {data(), size_};
        }

        constexpr MutableArrayView2D<T> mut() noexcept
        {
            return {data(), size_};
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
        ArrayView2D<T>
        subarray(Extent2D<size_t> extent) const
        {
            extent = clamp(extent, size_);
            return {
                data() + extent.origin.row * col_count() + extent.origin.column,
                extent.size,
                col_count() - extent.size.columns
            };
        }

        [[nodiscard]]
        MutableArrayView2D<T>
        subarray(Extent2D<size_t> extent)
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

        void resize(Size2D<size_t> size)
        {
            auto old_value_count = value_count();
            auto old_size = size_;
            auto value_count = size.rows * size.columns;
            buffer_.resize(value_count);
            size_ = size;

            if (old_size.rows == 0)
                return;

            if (size.columns > old_size.columns)
            {
                auto src = buffer_.data() + old_value_count - 1;
                auto dst = buffer_.data() + (old_size.rows - 1) * size.columns + old_size.columns -
                    1;
                while (src != dst)
                {
                    for (size_t i = 0; i < old_size.columns; ++i)
                        *dst-- = *src--;
                    for (size_t i = 0; i < size.columns - old_size.columns; ++i)
                        *dst-- = 0;
                }
            }
            else if (size.columns < old_size.columns)
            {
                auto src = buffer_.data() + old_size.columns;
                auto dst = buffer_.data() + size.columns;
                for (size_t i = 0; i < old_size.rows - 1; ++i)
                {
                    for (size_t j = 0; j < size.columns; ++j)
                        *dst++ = *src++;
                    src += old_size.columns - size.columns;
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
            size_.rows = size_.columns = 0;
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
            return a.size_ == b.size_
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
        Size2D<size_t> size_;
    };
}
