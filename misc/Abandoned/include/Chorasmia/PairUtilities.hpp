//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <utility>
#include <vector>

namespace Ylgorithm {

    template <typename T, typename U>
    struct Firster
    {
        const T& operator()(const std::pair<T, U>& p) const
        {
            return p.first;
        }
    };

    template <typename T, typename U>
    Firster<T, U> firster(const std::vector<std::pair<T, U>>&)
    {
        return Firster<T, U>();
    }

    template <typename T, typename U>
    Firster<T, U> firster(const std::pair<T, U>*)
    {
        return Firster<T, U>();
    }

    template <typename T, typename U>
    struct Seconder
    {
        const U& operator()(const std::pair<T, U>& p) const
        {
            return p.second;
        }
    };

    template <typename T, typename U>
    Seconder<T, U> seconder(const std::vector<std::pair<T, U>>&)
    {
        return Seconder<T, U>();
    }

    template <typename T, typename U>
    Seconder<T, U> seconder(const std::pair<T, U>*)
    {
        return Seconder<T, U>();
    }
}
