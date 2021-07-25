//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-14.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/ArrayView2DAlgorithms.hpp>
#include <catch2/catch.hpp>
#include <Chorasmia/Array2D.hpp>

TEST_CASE("Test copy")
{
    using namespace Chorasmia;
    Array2D<int> a({1, 2, 3, 4, 5, 6}, 2, 3);
    Array2D<int> b(3, 2);
    copy<int>(a, b, Index2DMode::REVERSED_COLUMNS_REVERSED_ORDER);
    REQUIRE(b(0, 0) == 6);
    REQUIRE(b(0, 1) == 3);
    REQUIRE(b(1, 0) == 5);
    REQUIRE(b(1, 1) == 2);
    REQUIRE(b(2, 0) == 4);
    REQUIRE(b(2, 1) == 1);
}
