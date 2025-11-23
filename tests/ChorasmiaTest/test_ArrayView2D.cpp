//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Chorasmia/ArrayView2D.hpp"
#include <numeric>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ArrayView2D initialization.")
{
    std::vector<int32_t> values = {
        0, 2, 0,
        3, 4, 3,
        0, 2, 0
    };

    Chorasmia::ArrayView2D<int32_t> grid(values.data(), {3, 3});
    REQUIRE(grid.row_count() == 3);
    REQUIRE(grid.col_count() == 3);
    REQUIRE(grid.col_count() == 3);
    REQUIRE(grid[{0, 0}] == values[0]);
    REQUIRE(grid[{1, 0}] == values[3]);
    REQUIRE(grid[{2, 1}] == values[7]);
    REQUIRE(grid[{2, 2}] == values[8]);
}

TEST_CASE("ArrayView2D iterator.")
{
    std::vector<int32_t> values(12);
    std::iota(values.begin(), values.end(), 0);

    Chorasmia::ArrayView2D grid(values.data(), {4, 3});
    int32_t i = 0;
    for (auto& row : grid)
    {
        for (auto v : row)
        {
            REQUIRE(v == i++);
        }
    }
}

TEST_CASE("ArrayView2D equality.")
{
    using namespace Chorasmia;
    std::vector<int32_t> a(12);
    REQUIRE(ArrayView2D(a.data(), {3, 4}) == ArrayView2D(a.data(), {3, 4}));
    REQUIRE(ArrayView2D(a.data(), {3, 4}) != ArrayView2D(a.data(), {4, 3}));
    std::vector<int32_t> b(12);
    REQUIRE(ArrayView2D(a.data(), {3, 4}) == ArrayView2D(b.data(), {3, 4}));
    b[3] = 6;
    REQUIRE(ArrayView2D(a.data(), {3, 4}) != ArrayView2D(b.data(), {3, 4}));
}

TEST_CASE("ArrayView2D contiguous.")
{
    using namespace Chorasmia;
    std::vector<int32_t> a(12);
    REQUIRE(ArrayView2D(a.data(), {3, 4}).contiguous());
    REQUIRE_FALSE(ArrayView2D(a.data(), {3, 2}, 2).contiguous());
    REQUIRE(ArrayView2D(a.data(), {1, 2}, 2).contiguous());
}

TEST_CASE("ArrayView2D subarray indexing.")
{
    using namespace Chorasmia;
    std::vector<int32_t> a(12);
    ArrayView2D grid(a.data(), {3, 4});
    auto sub1 = grid.subarray({{1, 1}, {2, 2}});
    REQUIRE(sub1.row_count() == 2);
    REQUIRE(sub1.col_count() == 2);
    REQUIRE(sub1[{0, 0}] == a[5]);
    REQUIRE(sub1[{1, 1}] == a[10]);
    auto sub2 = grid.subarray(Extent2D<size_t>({1, 2}));
    REQUIRE(sub2.row_count() == 2);
    REQUIRE(sub2.col_count() == 2);
    REQUIRE(sub2[{0, 0}] == a[6]);
    REQUIRE(sub2[{1, 1}] == a[11]);
}
