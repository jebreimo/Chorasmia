//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

namespace Chorasmia
{
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    struct Approx
    {
        constexpr static T DEFAULT_MARGIN = 100 * std::numeric_limits<T>::epsilon();

        explicit constexpr Approx(T value, T margin = DEFAULT_MARGIN)
            : value(value),
              margin(margin)
        {}

        T value;
        T margin;
    };

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator==(U v, Approx<T> m)
    {
        return std::abs(m.value - v) <= m.margin;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator==(Approx<T> m, U v)
    {
        return std::abs(m.value - v) <= m.margin;
    }

    template <typename T, typename U>
    constexpr bool operator==(Approx<T> a, Approx<U> b)
    {
        using V = decltype(T() + U());
        return Approx<V>(a.value, std::max<V>(a.margin, b.margin)) == b.value;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator!=(U v, Approx<T> m)
    {
        return !(v == m);
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator!=(Approx<T> m, U v)
    {
        return !(m == v);
    }

    template <typename T, typename U>
    constexpr bool operator!=(Approx<T> a, Approx<U> b)
    {
        using V = decltype(T() + U());
        return Approx<V>(a.value, std::max<V>(a.margin, b.margin)) != b.value;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator<(Approx<T> m, U v)
    {
        return m.value + m.margin < v;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator<(U v, Approx<T> m)
    {
        return m.value - m.margin > v;
    }

    template <typename T, typename U>
    constexpr bool operator<(Approx<T> a, Approx<U> b)
    {
        using V = decltype(T() + U());
        return Approx<V>(a.value, std::max<V>(a.margin, b.margin)) < b.value;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator>(Approx<T> m, U v)
    {
        return v < m;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator>(U v, Approx<T> m)
    {
        return m < v;
    }

    template <typename T, typename U>
    constexpr bool operator>(Approx<T> a, Approx<U> b)
    {
        using V = decltype(T() + U());
        return Approx<V>(a.value, std::max<V>(a.margin, b.margin)) > b.value;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator<=(Approx<T> m, U v)
    {
        return !(v < m);
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator<=(U v, Approx<T> m)
    {
        return !(m < v);
    }

    template <typename T, typename U>
    constexpr bool operator<=(Approx<T> a, Approx<U> b)
    {
        using V = decltype(T() + U());
        return Approx<V>(a.value, std::max<V>(a.margin, b.margin)) <= b.value;
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator>=(Approx<T> m, U v)
    {
        return !(m < v);
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator>=(U v, Approx<T> m)
    {
        return !(v < m);
    }

    template <typename T, typename U>
    constexpr bool operator>=(Approx<T> a, Approx<U> b)
    {
        using V = decltype(T() + U());
        return Approx<V>(a.value, std::max<V>(a.margin, b.margin)) >= b.value;
    }
}
