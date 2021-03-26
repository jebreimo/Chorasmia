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

namespace Chorasmia
{
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    struct Approx
    {
        explicit constexpr Approx(T value, T margin = 100 * std::numeric_limits<T>::epsilon())
            : value(value),
              margin(margin)
        {}

        T value;
        T margin;
    };

    template <typename T>
    constexpr bool operator==(T v, Approx<T> m)
    {
        return std::abs(m.value - v) <= m.margin;
    }

    template <typename T>
    constexpr bool operator==(Approx<T> m, T v)
    {
        return std::abs(m.value - v) <= m.margin;
    }

    template <typename T>
    constexpr bool operator==(Approx<T> a, Approx<T> b)
    {
        return Approx<T>(a.value, std::max(a.margin, b.margin)) == b.value;
    }

    template <typename T>
    constexpr bool operator!=(T v, Approx<T> m)
    {
        return !(v == m);
    }

    template <typename T>
    constexpr bool operator!=(Approx<T> m, T v)
    {
        return !(m == v);
    }

    template <typename T>
    constexpr bool operator!=(Approx<T> a, Approx<T> b)
    {
        return Approx<T>(a.value, std::max(a.margin, b.margin)) != b.value;
    }

    template <typename T>
    constexpr bool operator<(Approx<T> m, T v)
    {
        return m.value + m.margin < v;
    }

    template <typename T>
    constexpr bool operator<(T v, Approx<T> m)
    {
        return m.value - m.margin > v;
    }

    template <typename T>
    constexpr bool operator<(Approx<T> a, Approx<T> b)
    {
        return Approx<T>(a.value, std::max(a.margin, b.margin)) < b.value;
    }

    template <typename T>
    constexpr bool operator>(Approx<T> m, T v)
    {
        return v < m;
    }

    template <typename T>
    constexpr bool operator>(T v, Approx<T> m)
    {
        return m < v;
    }

    template <typename T>
    constexpr bool operator>(Approx<T> a, Approx<T> b)
    {
        return Approx<T>(a.value, std::max(a.margin, b.margin)) > b.value;
    }

    template <typename T>
    constexpr bool operator<=(Approx<T> m, T v)
    {
        return !(v < m);
    }

    template <typename T>
    constexpr bool operator<=(T v, Approx<T> m)
    {
        return !(m < v);
    }

    template <typename T>
    constexpr bool operator<=(Approx<T> a, Approx<T> b)
    {
        return Approx<T>(a.value, std::max(a.margin, b.margin)) <= b.value;
    }

    template <typename T>
    constexpr bool operator>=(Approx<T> m, T v)
    {
        return !(m < v);
    }

    template <typename T>
    constexpr bool operator>=(T v, Approx<T> m)
    {
        return !(v < m);
    }

    template <typename T>
    constexpr bool operator>=(Approx<T> a, Approx<T> b)
    {
        return Approx<T>(a.value, std::max(a.margin, b.margin)) >= b.value;
    }
}
