//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstddef>

namespace Chorasmia
{
    template <typename T>
    class ArrayView
    {
    public:
        constexpr ArrayView() = default;

        constexpr ArrayView(const T* data, size_t size) noexcept
            : data_(data),
              size_(size)
        {}

        [[nodiscard]]
        const T& operator[](size_t i) const
        {
            return data_[i];
        }

        [[nodiscard]]
        const T* begin() const
        {
            return data_;
        }

        [[nodiscard]]
        const T* end() const
        {
            return data_ + size_;
        }

        [[nodiscard]]
        constexpr size_t size() const
        {
            return size_;
        }

        [[nodiscard]]
        const T* data() const
        {
            return data_;
        }

        constexpr ArrayView<T>
        subarray(size_t offset, size_t length = SIZE_MAX) const noexcept
        {
            return {data_ + offset, std::min(size_ - offset, length)};
        }

        [[nodiscard]]
        friend bool operator==(const ArrayView& a, const ArrayView& b)
        {
            return a.size() == b.size()
                   && (a.data() == b.data()
                       || std::equal(a.begin(), a.end(), b.begin()));
        }

        [[nodiscard]]
        friend bool operator!=(const ArrayView& a, const ArrayView& b)
        {
            return !(a == b);
        }

    private:
        const T* data_ = nullptr;
        size_t size_ = 0;
    };

    template <typename T>
    [[nodiscard]]
    constexpr bool are_identical(const ArrayView<T>& a, const ArrayView<T>& b)
    {
        return a.data() == b.data() && a.size() == b.size();
    }
}
