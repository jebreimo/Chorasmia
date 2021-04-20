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

    template <MatrixPath method>
    constexpr std::pair<size_t, size_t>
    mapSize(std::pair<size_t, size_t> size)
    {
        if constexpr (isRowMajor(method))
            return size;
        else
            return {size.second, size.first};
    }

    constexpr std::pair<size_t, size_t>
    mapSize(std::pair<size_t, size_t> size, MatrixPath method)
    {
        if (isRowMajor(method))
            return size;
        else
            return {size.second, size.first};
    }

    //template <MatrixPath method>
    //constexpr std::pair<size_t, size_t>
    //getFromIndex(const std::pair<size_t, size_t>& size, size_t i, size_t j)
    //{
    //    if constexpr (method == MatrixPath::UP_RIGHT)
    //        return {size.second - 1 - j, i};
    //    if constexpr (method == MatrixPath::LEFT_UP)
    //        return {size.first - 1 - i, size.second - j};
    //    if constexpr (method == MatrixPath::DOWN_LEFT)
    //        return {j, size.first - i};
    //    if constexpr (method == MatrixPath::DOWN_RIGHT)
    //        return {j, i};
    //    if constexpr (method == MatrixPath::RIGHT_UP)
    //        return {size.first - 1 - i, j};
    //    if constexpr (method == MatrixPath::UP_LEFT)
    //        return {size.second - 1 - j, size.first - 1 - i};
    //    if constexpr (method == MatrixPath::LEFT_DOWN)
    //        return {i, size.second - 1 - j};
    //    return {i, j};
    //};

    constexpr std::pair<size_t, size_t>
    getFromIndex(const std::pair<size_t, size_t>& size, size_t i, size_t j,
                 MatrixPath path)
    {
        switch (path)
        {
        case MatrixPath::RIGHT_DOWN:
            return {i, j};
        case MatrixPath::UP_RIGHT:
            return {size.second - 1 - j, i};
        case MatrixPath::LEFT_UP:
            return {size.first - 1 - i, size.second - j};
        case MatrixPath::DOWN_LEFT:
            return {j, size.first - i};
        case MatrixPath::DOWN_RIGHT:
            return {j, i};
        case MatrixPath::RIGHT_UP:
            return {size.first - 1 - i, j};
        case MatrixPath::UP_LEFT:
            return {size.second - 1 - j, size.first - 1 - i};
        case MatrixPath::LEFT_DOWN:
            return {i, size.second - 1 - j};
        }
    }
}
