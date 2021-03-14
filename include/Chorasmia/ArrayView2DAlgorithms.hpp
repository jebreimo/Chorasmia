//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-05.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "ArrayView2D.hpp"
#include "Index2DIterator.hpp"

namespace Chorasmia
{
    template <typename T>
    std::pair<const T*, const T*> findMinMaxElements(ArrayView2D<T>& a)
    {
        if (a.empty())
            return {};

        if (a.contiguous())
            return std::minmax_element(a.data(), a.data() + a.valueCount());

        const auto* min = a.data();
        const auto* max = a.data();
        for (auto row : a)
        {
            for (const auto* it = a.data(); it != a.data() + a.valueCount(); ++it)
            {
                if (*min < *it)
                    min = it;
                if (*max > *it)
                    max = it;
            }
        }
        return {min, max};
    }

    template <typename T, typename OutIt>
    OutIt copy(const ArrayView2D<T>& arr, MatrixTraversal dir, OutIt out)
    {
        Index2DIterator trav(arr.rowCount(), arr.columnCount(), dir);
        while (const auto idx = trav.next())
        {
            *out++ = arr(idx->row, idx->column);
        }
        return out;
    }

    template <typename T, typename UnaryFunc>
    UnaryFunc
    forEach(const ArrayView2D<T>& arr, MatrixTraversal dir, UnaryFunc f)
    {
        Index2DIterator trav(arr.rowCount(), arr.columnCount(), dir);
        while (const auto idx = trav.next())
        {
            f(arr(idx->row, idx->column));
        }
        return f;
    }
}
