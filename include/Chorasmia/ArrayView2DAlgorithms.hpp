//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-05.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include "MutableArrayView2D.hpp"
#include "Index2DMap.hpp"

namespace Chorasmia
{
    template <typename T>
    std::pair<const T*, const T*> find_min_max_elements(ArrayView2D<T>& a)
    {
        if (a.empty())
            return {};

        if (a.contiguous())
            return std::minmax_element(a.data(), a.data() + a.value_count());

        const auto* min = a.data();
        const auto* max = a.data();
        for (auto row : a)
        {
            for (const auto* it = a.data(); it != a.data() + a.value_count(); ++it)
            {
                if (*min < *it)
                    min = it;
                if (*max > *it)
                    max = it;
            }
        }
        return {min, max};
    }

    template <typename T>
    void copy(const ArrayView2D<T>& src,
              const MutableArrayView2D<T>& dst,
              Index2DMode path)
    {
        Index2DMap mapping(src.dimensions(), path);
        if (mapping.get_to_size() != dst.dimensions())
            CHORASMIA_THROW("dst has incorrect dimensions.");
        const auto[m, n] = mapping.get_to_size();
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                dst(i, j) = src(mapping.get_from_indices(i, j));
            }
        }
    }
}
