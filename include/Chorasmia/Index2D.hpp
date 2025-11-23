//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-11-22.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <ostream>
#include "SaturationMath.hpp"

namespace Chorasmia
{
    template <std::integral T>
    struct Index2D
    {
        constexpr Index2D() noexcept
            : row(0), column(0)
        {}

        constexpr Index2D(T row, T column) noexcept
            : row(row), column(column)
        {}

        static constexpr Index2D max()
        {
            return {
                std::numeric_limits<T>::max(),
                std::numeric_limits<T>::max()
            };
        }

        union
        {
            struct
            {
                T row;
                T column;
            };

            struct
            {
                T rows;
                T columns;
            };
        };
    };

    template <std::integral T>
    using Size2D = Index2D<T>;

    template <std::integral T>
    constexpr bool operator==(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return a.row == b.row && a.column == b.column;
    }

    template <std::integral T>
    constexpr bool operator!=(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return !(a == b);
    }

    template <std::integral T>
    constexpr Index2D<T> operator+(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return {
            saturating_add(a.row, b.row),
            saturating_add(a.column, b.column)
        };
    }

    template <std::integral T>
    constexpr Index2D<T> operator-(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return {
            saturating_subtract(a.row, b.row),
            saturating_subtract(a.column, b.column)
        };
    }

    template <std::integral T>
    constexpr Index2D<T> operator*(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return {
            saturating_multiply(a.row, b.row),
            saturating_multiply(a.column, b.column)
        };
    }

    template <std::integral T>
    constexpr Index2D<T> operator/(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return {
            saturating_divide(a.row, b.row),
            saturating_divide(a.column, b.column)
        };
    }

    template <std::integral T>
    constexpr Index2D<T> operator*(const Index2D<T>& index, T scalar) noexcept
    {
        return {
            saturating_multiply(index.row, scalar),
            saturating_multiply(index.column, scalar)
        };
    }

    template <std::integral T>
    constexpr Index2D<T> operator*(T scalar, const Index2D<T>& index) noexcept
    {
        return {
            saturating_multiply(index.row, scalar),
            saturating_multiply(index.column, scalar)
        };
    }

    template <std::integral T>
    constexpr Index2D<T> operator/(const Index2D<T>& index, T scalar) noexcept
    {
        return {
            saturating_divide(index.row, scalar),
            saturating_divide(index.column, scalar)
        };
    }

    template <std::integral T>
    constexpr Index2D<T> operator%(const Index2D<T>& index, T scalar) noexcept
    {
        return {index.row % scalar, index.column % scalar};
    }

    template <std::integral T>
    constexpr Index2D<T>& operator+=(Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        a = a + b;
        return a;
    }

    template <std::integral T>
    constexpr Index2D<T>& operator-=(Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        a.row = a - b;
        return a;
    }

    template <std::integral T>
    constexpr Index2D<T>& operator*=(Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        a.row = a * b;
        return a;
    }

    template <std::integral T>
    constexpr Index2D<T>& operator/=(Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        a.row = a / b;
        return a;
    }

    template <std::integral T>
    constexpr Index2D<T>& operator*=(Index2D<T>& index, T scalar) noexcept
    {
        index = index * scalar;
        return index;
    }

    template <std::integral T>
    constexpr Index2D<T>& operator/=(Index2D<T>& index, T scalar) noexcept
    {
        index = index / scalar;
        return index;
    }

    template <std::integral T>
    constexpr Index2D<T>& operator%=(Index2D<T>& index, T scalar) noexcept
    {
        index.row %= scalar;
        index.column %= scalar;
        return index;
    }

    template <std::integral T>
    std::ostream& operator<<(std::ostream& os, const Index2D<T>& index)
    {
        return os << '{' << index.row << ", " << index.column << '}';
    }

    template <std::integral T>
    constexpr Index2D<T> get_min(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return {std::min(a.row, b.row), std::min(a.column, b.column)};
    }

    template <std::integral T>
    constexpr Index2D<T> get_max(const Index2D<T>& a, const Index2D<T>& b) noexcept
    {
        return {std::max(a.row, b.row), std::max(a.column, b.column)};
    }

    template <std::integral T1, std::integral T2>
    [[nodiscard]]
    constexpr Index2D<T1> cast(const Index2D<T2>& index) noexcept
    {
        return {static_cast<T1>(index.row), static_cast<T1>(index.column)};
    }

    template <std::integral T>
    constexpr bool is_empty(const Size2D<T>& size) noexcept
    {
        return size.rows == 0 || size.columns == 0;
    }

    template <unsigned Index, std::integral T>
    [[nodiscard]] constexpr T get(const Index2D<T>& index) noexcept
    {
        static_assert(Index < 2, "Incorrect vector index.");
        if constexpr (Index == 0)
            return index.row;
        else
            return index.column;
    }

    template <unsigned Index, std::integral T>
    [[nodiscard]] constexpr T& get(Index2D<T>& index) noexcept
    {
        static_assert(Index < 2, "Incorrect vector index.");
        if constexpr (Index == 0)
            return index.row;
        else
            return index.column;
    }
}

template <std::integral T>
struct std::tuple_size<Chorasmia::Index2D<T>>
    : std::integral_constant<unsigned, 2>
{};

template <size_t I, std::integral T>
struct std::tuple_element<I, Chorasmia::Index2D<T>>
{
    using type = T;
};
