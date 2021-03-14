//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <optional>
#include "Index2D.hpp"

namespace Chorasmia
{
    class BasicIndex2DIterator
    {
    public:
        BasicIndex2DIterator(size_t rows, size_t columns)
            : m_Rows(rows),
              m_Columns(columns)
        {}

        std::optional<Index2D> next()
        {
            if (++m_Col < m_Columns)
                return {{m_Row, m_Col}};

            if (++m_Row < m_Rows)
            {
                m_Col = 0;
                return {{m_Row, m_Col}};
            }

            --m_Col;
            --m_Row;
            return {};
        }

        [[nodiscard]]
        constexpr size_t rows() const
        {
            return m_Rows;
        }

        [[nodiscard]]
        constexpr size_t columns() const
        {
            return m_Columns;
        }

    private:
        size_t m_Row = 0;
        size_t m_Col = SIZE_MAX;
        size_t m_Rows;
        size_t m_Columns;
    };

    enum class MatrixTraversal
    {
        RIGHT_DOWN, /// The normal matrix traversal
        UP_RIGHT,
        LEFT_UP,
        DOWN_LEFT,
        DOWN_RIGHT, /// Transposed normal matrix traversal
        RIGHT_UP,
        UP_LEFT,
        LEFT_DOWN
    };

    constexpr bool isTransposed(MatrixTraversal dir)
    {
        return unsigned(dir) > unsigned(MatrixTraversal::DOWN_RIGHT);
    }

    constexpr bool isRowMajor(MatrixTraversal dir)
    {
        return isTransposed(dir) == (unsigned(dir) % 2 != 0);
    }

    constexpr MatrixTraversal transpose(MatrixTraversal dir)
    {
        return MatrixTraversal(unsigned(dir) ^ 4u);
    }

    constexpr MatrixTraversal
    rotateLeft(MatrixTraversal dir, int turns)
    {
        const auto transposed = unsigned(dir) & 4u;
        const auto angle = int(unsigned(dir) & 3u);
        const auto newAngle = unsigned(angle + turns) & 3u;
        return MatrixTraversal(newAngle | transposed);
    }

    class Index2DIterator
    {
    public:
        Index2DIterator(size_t rows, size_t columns,
                        MatrixTraversal direction)
            : m_It(isRowMajor(direction) ? rows : columns,
                   isRowMajor(direction) ? columns : rows),
              m_Direction(direction)
        {
        }

        std::optional<Index2D> next()
        {
            const auto index = m_It.next();
            if (!index)
                return {};

            auto[maj, min] = *index;

            switch (m_Direction)
            {
            case MatrixTraversal::RIGHT_DOWN:
                return {{maj, min}};
            case MatrixTraversal::UP_RIGHT:
                return {{m_It.columns() - 1 - min, maj}};
            case MatrixTraversal::LEFT_UP:
                return {{m_It.rows() - 1 - maj, m_It.columns() - 1 - min}};
            case MatrixTraversal::DOWN_LEFT:
                return {{min, m_It.rows() - 1 - maj}};
            case MatrixTraversal::DOWN_RIGHT:
                return {{min, maj}};
            case MatrixTraversal::RIGHT_UP:
                return {{m_It.rows() - 1 - maj, min}};
            case MatrixTraversal::UP_LEFT:
                return {{m_It.columns() - 1 - min, m_It.rows() - 1 - maj}};
            case MatrixTraversal::LEFT_DOWN:
                return {{maj, m_It.columns() - 1 - min}};
            }
        }

    private:
        BasicIndex2DIterator m_It;
        MatrixTraversal m_Direction;
    };
}
