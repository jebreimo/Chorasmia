//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-09.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <optional>

namespace Chorasmia
{
    struct Index2D
    {
        size_t row = 0;
        size_t column = 0;
    };

    constexpr bool operator==(const Index2D& a, const Index2D& b)
    {
        return a.row == b.row && a.column == b.column;
    }

    constexpr bool operator!=(const Index2D& a, const Index2D& b)
    {
        return !(a == b);
    }
}
