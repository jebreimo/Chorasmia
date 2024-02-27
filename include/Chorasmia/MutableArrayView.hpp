//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "ArrayView.hpp"

namespace Chorasmia
{
    template <typename T>
    class MutableArrayView
    {
    public:
        constexpr MutableArrayView() = default;

        constexpr MutableArrayView(T* data, size_t size) noexcept
            : data_(data),
              size_(size)
        {}

        [[nodiscard]]
        constexpr T& operator[](size_t i) const
        {
            return data_[i];
        }

        [[nodiscard]]
        constexpr T* begin() const
        {
            return data_;
        }

        [[nodiscard]]
        constexpr T* end() const
        {
            return data_ + size_;
        }

        [[nodiscard]]
        constexpr size_t size() const
        {
            return size_;
        }

        [[nodiscard]]
        constexpr T* data() const
        {
            return data_;
        }

        constexpr ArrayView<T> view() const noexcept
        {
            return ArrayView<T>(data_, size_);
        }

        constexpr MutableArrayView<T>
        subarray(size_t offset, size_t length = SIZE_MAX) const noexcept
        {
            return {data_ + offset, std::min(size_ - offset, length)};
        }

        [[nodiscard]]
        friend bool operator==(const MutableArrayView& a,
                               const MutableArrayView& b)
        {
            return a.size() == b.size()
                   && (a.data() == b.data()
                       || std::equal(a.begin(), a.end(), b.begin()));
        }

        [[nodiscard]]
        friend bool operator!=(const MutableArrayView& a,
                               const MutableArrayView& b)
        {
            return !(a == b);
        }

    private:
        T* data_ = nullptr;
        size_t size_ = 0;
    };

    template <typename T>
    [[nodiscard]]
    constexpr bool are_identical(const MutableArrayView<T>& a,
                                 const MutableArrayView<T>& b)
    {
        return a.data() == b.data() && a.size() == b.size();
    }
}
