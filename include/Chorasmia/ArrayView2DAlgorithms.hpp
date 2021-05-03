//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-05.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "ArrayView2D.hpp"
#include "Index2DMap.hpp"

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
    OutIt copy(const ArrayView2D<T>& arr, Index2DMode path, OutIt out)
    {
        Index2DMap mapping(arr.dimensions(), path);
        const auto [m, n] = mapping.getToSize();
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                *out++ = arr(mapping.getFromIndices(i, j));
            }
        }
        return out;
    }
}
