//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstddef>
#include <utility>

namespace Chorasmia
{
    enum class Index2DMode
    {
        ROWS, /// The normal (row-wise) matrix traversal
        REVERSED_ROWS,
        ROWS_REVERSED_ORDER,
        REVERSED_ROWS_REVERSED_ORDER,
        COLUMNS, /// Transposed (column-wise) matrix traversal
        COLUMNS_REVERSED_ORDER,
        REVERSED_COLUMNS,
        REVERSED_COLUMNS_REVERSED_ORDER
    };

    constexpr bool is_transposed(Index2DMode path)
    {
        return (0b10010110u & (1u << unsigned(path))) != 0;
    }

    constexpr bool is_row_major(Index2DMode path)
    {
        return unsigned(path) < 4;
    }

    constexpr Index2DMode transpose(Index2DMode path)
    {
        return Index2DMode(unsigned(path) ^ 4u);
    }

    constexpr Index2DMode invert(Index2DMode path)
    {
        if (((unsigned(path) + 1u) & 0b110u) != 0b110)
            return path;
        else
            return Index2DMode(unsigned(path) ^ 0b11u);
    }

    constexpr Index2DMode rotate_ccw(Index2DMode path, int turns)
    {
        unsigned mask = 0;
        unsigned b = unsigned(path) & 4u;
        if (turns < 0)
            turns = 4 - (-turns % 4);

        switch (turns % 4)
        {
        default: break;
        case 1: mask = (4u | (b >> 1u) | ((b >> 2u) ^ 1u)); break;
        case 2: mask = 3; break;
        case 3: mask = (4u | ((b >> 1u) ^ 2u) | (b >> 2u)); break;
        }

        return Index2DMode(unsigned(path) ^ mask);
    }

    class Index2DMap
    {
    public:
        constexpr Index2DMap(
            std::pair<size_t, size_t> fromSize,
            Index2DMode path)
            : from_size_(std::move(fromSize)),
              path_(path),
              inverse_path_(invert(path))
        {}

        [[nodiscard]]
        constexpr std::pair<size_t, size_t>
        get_from_indices(size_t i, size_t j) const
        {
            auto u = unsigned(inverse_path_);
            if (is_row_major(inverse_path_))
            {
                return {(u & 0b10u) ? from_size_.first - 1 - i : i,
                        (u & 1u) ? from_size_.second - 1 - j : j};
            }
            else
            {
                return {(u & 1u) ? from_size_.first - 1 - j : j,
                        (u & 0b10u) ? from_size_.second - 1 - i : i};
            }
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t>
        get_to_indices(size_t i, size_t j) const
        {
            auto u = unsigned(path_);
            if (is_row_major(path_))
            {
                return {(u & 0b10u) ? from_size_.first - 1 - i : i,
                        (u & 1u) ? from_size_.second - 1 - j : j};
            }
            else
            {
                return {(u & 1u) ? from_size_.second - 1 - j : j,
                        (u & 0b10u) ? from_size_.first - 1 - i : i};
            }
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> get_from_size() const
        {
            return from_size_;
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> get_to_size() const
        {
            if (is_row_major(path_))
                return from_size_;
            else
                return {from_size_.second, from_size_.first};
        }

    private:
        std::pair<size_t, size_t> from_size_;
        Index2DMode path_;
        Index2DMode inverse_path_;
    };
}
