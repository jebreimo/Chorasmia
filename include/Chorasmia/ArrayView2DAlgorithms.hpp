//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-05.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <cmath>
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

    /**
     * @brief A concept for types that can be multiplied by a scalar.
     */
    template <typename T>
    concept ScalarMultipliable = requires(T a, double b)
    {
        { a * b } -> std::convertible_to<T>;
    };

    /**
     * @brief A concept for types that can be added to each other.
     */
    template <typename T>
    concept Addable = requires(T a, T b)
    {
        { a + b } -> std::convertible_to<T>;
    };

    // A concept for types that are Addable and ScalarMultipliable.
    template <typename T>
    concept AddableAndScalarMultipliable = Addable<T> && ScalarMultipliable<T>;

    /**
     * @brief Interpolates a value in a 2D array using bilinear interpolation.
     * @param array The array to interpolate in.
     * @param i The row index of the point to interpolate.
     * @param j The column index of the point to interpolate.
     * @return The interpolated value.
     */
    template <AddableAndScalarMultipliable T>
    T interpolate_value(const ArrayView2D<T>& array, double i, double j)
    {
        bool on_boundary = false;

        double i_int;
        auto i_frac = std::modf(i, &i_int);
        const auto i_idx = size_t(i_int);
        if (i_int < 0 || i_frac < 0 || array.row_count() - 1 <= i_idx)
        {
            if (array.row_count() - 1 != i_idx || i_frac != 0)
                CHORASMIA_THROW("i lies outside the array.");
            on_boundary = true;
        }

        double j_int;
        auto j_frac = std::modf(j, &j_int);
        const auto j_idx = size_t(j_int);
        if (j_int < 0 || j_frac < 0 || array.col_count() - 1 <= j_idx)
        {
            if (array.col_count() - 1 != j_idx || j_frac != 0)
                CHORASMIA_THROW("j lies outside the array.");
            on_boundary = true;
        }

        double weights[4] =
            {
                (1 - i_frac) * (1 - j_frac),
                i_frac * (1 - j_frac),
                (1 - i_frac) * j_frac,
                i_frac * j_frac
            };

        if (!on_boundary)
        {
            // If we're not on the boundary we can use all the array values.
            return array(i_idx, j_idx) * weights[0]
                   + array(i_idx + 1, j_idx) * weights[1]
                   + array(i_idx, j_idx + 1) * weights[2]
                   + array(i_idx + 1, j_idx + 1) * weights[3];
        }

        // If we're on the boundary we only get the array values that are
        // inside the array.
        T result = {};
        if (weights[0] != 0)
            result += array(i_idx, j_idx) * weights[0];
        if (weights[1] != 0)
            result += array(i_idx + 1, j_idx) * weights[1];
        if (weights[2] != 0)
            result += array(i_idx, j_idx + 1) * weights[2];
        if (weights[3] != 0)
            result += array(i_idx + 1, j_idx + 1) * weights[3];
        return result;
    }
}
