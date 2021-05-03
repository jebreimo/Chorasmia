//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <optional>

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

    constexpr bool isTransposed(Index2DMode path)
    {
        return (0b10010110u & (1u << unsigned(path))) != 0;
    }

    constexpr bool isRowMajor(Index2DMode path)
    {
        return unsigned(path) < 4;
    }

    constexpr Index2DMode transpose(Index2DMode path)
    {
        return Index2DMode(unsigned(path) ^ 4u);
    }

    constexpr Index2DMode invert(Index2DMode path)
    {
        if ((unsigned(path) + 1u & 0b110u) != 0b110)
            return path;
        else
            return Index2DMode(unsigned(path) ^ 0b11u);
    }

    constexpr Index2DMode
    rotateLeft(Index2DMode path, int turns)
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
            : m_FromSize(std::move(fromSize)),
              m_Path(path),
              m_InversePath(invert(path))
        {}

        [[nodiscard]]
        constexpr std::pair<size_t, size_t>
        getFromIndices(size_t i, size_t j) const
        {
            auto u = unsigned(m_InversePath);
            if (isRowMajor(m_InversePath))
            {
                return {(u & 0b10u) ? m_FromSize.first - 1 - i : i,
                        (u & 1u) ? m_FromSize.second - 1 - j : j};
            }
            else
            {
                return {(u & 1u) ? m_FromSize.first - 1 - j : j,
                        (u & 0b10u) ? m_FromSize.second - 1 - i : i};
            }
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t>
        getToIndices(size_t i, size_t j) const
        {
            auto u = unsigned(m_Path);
            if (isRowMajor(m_Path))
            {
                return {(u & 0b10u) ? m_FromSize.first - 1 - i : i,
                        (u & 1u) ? m_FromSize.second - 1 - j : j};
            }
            else
            {
                return {(u & 1u) ? m_FromSize.second - 1 - j : j,
                        (u & 0b10u) ? m_FromSize.first - 1 - i : i};
            }
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> getFromSize() const
        {
            return m_FromSize;
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> getToSize() const
        {
            if (isRowMajor(m_Path))
                return m_FromSize;
            else
                return {m_FromSize.second, m_FromSize.first};
        }

    private:
        std::pair<size_t, size_t> m_FromSize;
        Index2DMode m_Path;
        Index2DMode m_InversePath;
    };
}
