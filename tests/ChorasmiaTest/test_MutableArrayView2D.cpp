//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Chorasmia/MutableArrayView2D.hpp"
#include <catch2/catch_test_macros.hpp>

#include <numeric>
#include <vector>

TEST_CASE("MutableArrayView2D subarray")
{
    std::vector<int> values(16);
    Chorasmia::MutableArrayView2D grid(values.data(), 4, 4);
    REQUIRE(grid.contiguous());

    [[maybe_unused]]
    Chorasmia::ArrayView<int> a;
    REQUIRE_NOTHROW(a = grid.array().view());
    auto subgrid = grid.subarray(2, 2, 2, 2);
    REQUIRE(subgrid.row_count() == 2);
    REQUIRE(subgrid.col_count() == 2);
    REQUIRE(!subgrid.contiguous());
    REQUIRE_THROWS(a = subgrid.array().view());
    REQUIRE(grid(3, 3) == 0);
    subgrid(1, 1) = 7;
    REQUIRE(grid(3, 3) == 7);
}

TEST_CASE("MutableArrayView2D subarray iterator")
{
    std::vector<int> values(16);
    Chorasmia::MutableArrayView2D grid(values.data(), 4, 4);
    auto subgrid = grid.subarray(2, 2, 2, 2);
    int n = 1;
    for (auto row : subgrid)
    {
        std::iota(row.begin(), row.end(), n);
        n += int(std::distance(row.begin(), row.end()));
    }
    REQUIRE(grid(2, 2) == 1);
    REQUIRE(grid(2, 3) == 2);
    REQUIRE(grid(3, 2) == 3);
    REQUIRE(grid(3, 3) == 4);
}
