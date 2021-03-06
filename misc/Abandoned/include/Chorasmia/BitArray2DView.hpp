//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <cstddef>
#include <utility>

namespace GridLib
{
    class BitArray2DView
    {
    public:
        BitArray2DView() = default;

        BitArray2DView(const uint32_t* bits, unsigned rows, unsigned columns)
            : m_Bits(bits),
              m_Size(rows, columns)
        {}

        [[nodiscard]]
        bool get(size_t row, size_t column) const
        {
            auto i = row * columns() + column;
            auto iWord = i / 32;
            auto iBit = i % 32;
            return (m_Bits[iWord] & (1u << iBit)) != 0;
        }

        [[nodiscard]]
        bool empty() const
        {
            return m_Size == std::pair(size_t(0), size_t(0));
        }

        [[nodiscard]]
        size_t size() const
        {
            return m_Size.first * m_Size.second;
        }

        [[nodiscard]]
        size_t rows() const
        {
            return m_Size.first;
        }

        [[nodiscard]]
        size_t columns() const
        {
            return m_Size.second;
        }
    private:
        const uint32_t* m_Bits = nullptr;
        std::pair<size_t, size_t> m_Size;
    };
}
