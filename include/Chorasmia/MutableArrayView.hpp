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
            : m_Data(data),
              m_Size(size)
        {}

        constexpr operator ArrayView<T>() const noexcept
        {
            return ArrayView<T>(m_Data, m_Size);
        }

        [[nodiscard]]
        constexpr T& operator[](size_t i) const
        {
            return m_Data[i];
        }

        [[nodiscard]]
        constexpr T* begin() const
        {
            return m_Data;
        }

        [[nodiscard]]
        constexpr T* end() const
        {
            return m_Data + m_Size;
        }

        [[nodiscard]]
        constexpr size_t size() const
        {
            return m_Size;
        }

        [[nodiscard]]
        constexpr T* data() const
        {
            return m_Data;
        }

        constexpr ArrayView<T> immutable() const noexcept
        {
            return ArrayView<T>(m_Data, m_Size);
        }

        constexpr MutableArrayView<T>
        subarray(size_t offset, size_t length = SIZE_MAX) const noexcept
        {
            return {m_Data + offset, std::min(m_Size - offset, length)};
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
        T* m_Data = nullptr;
        size_t m_Size = 0;
    };

    template <typename T>
    [[nodiscard]]
    constexpr bool areIdentical(const MutableArrayView<T>& a,
                                const MutableArrayView<T>& b)
    {
        return a.data() == b.data() && a.size() == b.size();
    }
}
