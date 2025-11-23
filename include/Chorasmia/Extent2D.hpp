//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-11-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Index2D.hpp"

namespace Chorasmia
{
    template <std::integral T>
    struct Extent2D
    {
        constexpr Extent2D() noexcept = default;

        explicit constexpr Extent2D(Index2D<T> origin) noexcept
            : origin(origin), size(Size2D<T>::max())
        {}

        constexpr Extent2D(Index2D<T> origin, Size2D<T> size) noexcept
            : origin(origin), size(size)
        {}

        constexpr Index2D<T> min_index() const noexcept
        {
            return origin;
        }

        constexpr Index2D<T> max_index() const noexcept
        {
            return origin + size;
        }

        Index2D<T> origin;
        Size2D<T> size;
    };

    template <std::integral T>
    constexpr bool operator==(const Extent2D<T>& a, const Extent2D<T>& b) noexcept
    {
        return a.origin == b.origin && a.size == b.size;
    }

    template <std::integral T>
    constexpr bool operator!=(const Extent2D<T>& a, const Extent2D<T>& b) noexcept
    {
        return !(a == b);
    }

    template <std::integral T>
    std::ostream& operator<<(std::ostream& os, const Extent2D<T>& extent)
    {
        return os << '{' << extent.origin << ", " << extent.size << '}';
    }

    template <std::integral T>
    [[nodiscard]] constexpr std::optional<Extent2D<T>>
    get_intersection(const Extent2D<T>& a,
                     const Extent2D<std::type_identity_t<T>>& b) noexcept
    {
        const auto min = get_max(a.min_index(), b.min_index());
        const auto max = get_min(a.max_index(), b.max_index());
        if (min.row >= max.row || min.column >= max.column)
            return std::nullopt;

        return Extent2D<T>{min, max - min};
    }

    template <std::integral T>
    [[nodiscard]] constexpr Extent2D<T>
    get_union(const Extent2D<T>& a,
              const Extent2D<std::type_identity_t<T>>& b) noexcept
    {
        const auto min = get_min(a.min_index(), b.min_index());
        const auto max = get_max(a.max_index(), b.max_index());
        return {min, max - min};
    }

    template <std::integral T>
    constexpr bool is_empty(const Extent2D<T>& extent) noexcept
    {
        return is_empty(extent.size);
    }

    template <std::integral T>
    [[nodiscard]] constexpr Extent2D<T>
    clamp(const Extent2D<T>& extent,
          const Size2D<std::type_identity_t<T>>& size) noexcept
    {
        auto origin = get_min(extent.origin, size);
        return {origin, get_min(extent.size, size - origin)};
    }

    template <std::integral T1, std::integral T2>
    constexpr Extent2D<T1> cast(const Extent2D<T2>& extent) noexcept
    {
        return {cast<T1>(extent.origin), cast<T1>(extent.size)};
    }
}
