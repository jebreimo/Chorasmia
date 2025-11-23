//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-11-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>
#include <limits>
#include <type_traits>

namespace Chorasmia
{
    template <std::integral T>
    constexpr T saturating_add(T a, T b)
    {
        using Limit = std::numeric_limits<T>;

        if constexpr (std::is_unsigned_v<T>)
        {
            T result = a + b;
            if (result < a)
                return Limit::max();
            return result;
        }
        else
        {
            if ((b > 0) && (a > Limit::max() - b))
                return Limit::max();

            if ((b < 0) && (a < Limit::min() - b))
                return Limit::min();

            return a + b;
        }
    }

    template <std::integral T>
    constexpr T saturating_subtract(T a, T b)
    {
        using Limit = std::numeric_limits<T>;

        if constexpr (std::is_unsigned_v<T>)
        {
            if (b > a)
                return 0;
            return a - b;
        }
        else
        {
            if ((b < 0) && (a > Limit::max() + b))
                return Limit::max();

            if ((b > 0) && (a < Limit::min() + b))
                return Limit::min();

            return a - b;
        }
    }

    template <std::integral T>
    constexpr T saturating_multiply(T a, T b)
    {
        using Limit = std::numeric_limits<T>;

        if (a == 0 || b == 0)
            return 0;

        if constexpr (std::is_unsigned_v<T>)
        {
            if (a > Limit::max() / b)
                return Limit::max();
            return a * b;
        }
        else
        {
            bool negative_result = (a < 0) ^ (b < 0);
            T abs_a = a < 0 ? saturating_subtract<T>(0, a) : a;
            T abs_b = b < 0 ? saturating_subtract<T>(0, b) : b;

            if (abs_a > Limit::max() / abs_b)
                return negative_result ? Limit::min() : Limit::max();

            T result = abs_a * abs_b;
            return negative_result ? saturating_subtract<T>(0, result) : result;
        }
    }

    template <std::integral T>
    constexpr T saturating_divide(T a, T b)
    {
        using Limit = std::numeric_limits<T>;

        if (b == 0)
            return a >= 0 ? Limit::max() : Limit::min();

        if constexpr (std::is_unsigned_v<T>)
        {
            return a / b;
        }
        else
        {
            if (a == Limit::min() && b == -1)
                return Limit::max();
            return a / b;
        }
    }
}
