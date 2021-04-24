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
    enum class MatrixPath
    {
        RIGHT_DOWN, /// The normal (row-wise) matrix traversal
        UP_RIGHT,
        LEFT_UP,
        DOWN_LEFT,
        DOWN_RIGHT, /// Transposed (column-wise) matrix traversal
        RIGHT_UP,
        UP_LEFT,
        LEFT_DOWN
    };

    constexpr bool isTransposed(MatrixPath path)
    {
        return unsigned(path) > unsigned(MatrixPath::DOWN_RIGHT);
    }

    constexpr bool isRowMajor(MatrixPath path)
    {
        return isTransposed(path) == (unsigned(path) % 2 != 0);
    }

    constexpr MatrixPath transpose(MatrixPath path)
    {
        return MatrixPath(unsigned(path) ^ 4u);
    }

    constexpr MatrixPath invert(MatrixPath path)
    {
        if (unsigned(path) & 1)
            return MatrixPath(unsigned(path) ^ 2u);
        else
            return path;
    }

    constexpr MatrixPath
    rotateLeft(MatrixPath path, int turns)
    {
        const auto transposed = unsigned(path) & 4u;
        const auto angle = int(unsigned(path) & 3u);
        const auto newAngle = unsigned(angle + turns) & 3u;
        return MatrixPath(newAngle | transposed);
    }

    class MatrixIndexMapping
    {
    public:
        constexpr MatrixIndexMapping(
            const std::pair<size_t, size_t>& fromSize,
            MatrixPath path)
            : m_FromSize(fromSize),
              m_Path(path)
        {}

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> getFromIndices(size_t i, size_t j) const
        {
            switch (m_Path)
            {
            case MatrixPath::RIGHT_DOWN:
                return {i, j};
            case MatrixPath::UP_RIGHT:
                return {m_FromSize.first - 1 - j, i};
            case MatrixPath::LEFT_UP:
                return {m_FromSize.second - 1 - i, m_FromSize.first - 1 - j};
            case MatrixPath::DOWN_LEFT:
                return {j, m_FromSize.second - 1 - i};
            case MatrixPath::DOWN_RIGHT:
                return {j, i};
            case MatrixPath::RIGHT_UP:
                return {m_FromSize.second - 1 - i, j};
            case MatrixPath::UP_LEFT:
                return {m_FromSize.first - 1 - j, m_FromSize.second - 1 - i};
            case MatrixPath::LEFT_DOWN:
                return {i, m_FromSize.first - 1 - j};
            }
        }

        [[nodiscard]]
        constexpr std::pair<size_t, size_t> getToIndices(size_t i, size_t j) const
        {
            switch (m_Path)
            {
            case MatrixPath::RIGHT_DOWN:
                return {i, j};
            case MatrixPath::UP_RIGHT:
                return {j, m_FromSize.first - 1 - i};
            case MatrixPath::LEFT_UP:
                return {m_FromSize.first - 1 - i, m_FromSize.second - 1 - j};
            case MatrixPath::DOWN_LEFT:
                return {m_FromSize.second - 1 - j, i};
            case MatrixPath::DOWN_RIGHT:
                return {j, i};
            case MatrixPath::RIGHT_UP:
                return {i, m_FromSize.second - 1 - j};
            case MatrixPath::UP_LEFT:
                return {m_FromSize.second - 1 - j, m_FromSize.first - 1 - i};
            case MatrixPath::LEFT_DOWN:
                return {m_FromSize.first - 1 - i, j};
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
        MatrixPath m_Path;
    };
}
