//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <type_traits>
#include "ArrayView.hpp"
#include "MutableArrayView.hpp"

namespace GridLib
{
    template <typename T, bool IsMutable = false>
    class RowIterator
    {
    public:
        using ArrayViewType = typename std::conditional<IsMutable,
            MutableArrayView<T>, ArrayView<T>>::type;

        RowIterator() = default;

        constexpr explicit RowIterator(const ArrayViewType& row,
                                       size_t rowGap) noexcept
            : m_Row(row),
              m_RowGap(rowGap)
        {}

        RowIterator& operator++()
        {
            m_Row = ArrayViewType{m_Row.data() + rowSize(),
                                  m_Row.size()};
            return *this;
        }

        RowIterator operator++(int)
        {
            auto p = *this;
            m_Row = ArrayViewType{m_Row.data() + rowSize(),
                                  m_Row.size()};
            return p;
        }

        RowIterator& operator+=(size_t n)
        {
            m_Row = ArrayViewType{m_Row.data() + n * rowSize(),
                                  m_Row.size()};
            return *this;
        }

        RowIterator& operator--()
        {
            m_Row = {m_Row.data() - rowSize(), m_Row.size()};
            return *this;
        }

        RowIterator operator--(int)
        {
            auto p = *this;
            m_Row = ArrayViewType{m_Row.data() - rowSize(), m_Row.size()};
            return p;
        }

        RowIterator& operator-=(size_t n)
        {
            m_Row = ArrayViewType{m_Row.data() - n * rowSize(), m_Row.size()};
            return *this;
        }

        ArrayViewType operator[](size_t i) const
        {
            return ArrayViewType{m_Row.data() + i * rowSize(), m_Row.size()};
        }

        const ArrayViewType& operator*() const
        {
            return m_Row;
        }

        const ArrayViewType* operator->() const
        {
            return &m_Row;
        }

        [[nodiscard]]
        friend bool operator==(RowIterator a, RowIterator b)
        {
            return areIdentical(a, b);
        }

        [[nodiscard]]
        friend bool operator!=(RowIterator a, RowIterator b)
        {
            return !areIdentical(a, b);
        }
    private:
        [[nodiscard]]
        constexpr size_t rowSize() const
        {
            return m_Row.size() + m_RowGap;
        }

        ArrayViewType m_Row;
        size_t m_RowGap = 0;
    };
}
