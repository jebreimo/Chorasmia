//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-28.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <catch2/catch.hpp>
#include "Chorasmia/Array2D.hpp"

TEST_CASE("Array2DView")
{
    std::vector<int32_t> values = {
        0, 2, 0,
        3, 4, 3,
        0, 2, 0
    };

    GridLib::Array2DView<int32_t> grid(values.data(), 3, 3);
    REQUIRE(grid(0, 0) == values[0]);
    REQUIRE(grid(1, 0) == values[3]);
    REQUIRE(grid(2, 1) == values[7]);
    REQUIRE(grid(2, 2) == values[8]);
}

TEST_CASE("Add rows to Array2D")
{
    std::vector<int32_t> values = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    GridLib::Array2D<int32_t> grid(move(values), 3, 3);
    grid.resize(5, 3);
    REQUIRE(grid.rows() == 5);
    REQUIRE(grid.columns() == 3);
    REQUIRE(grid(0, 0) == 1);
    REQUIRE(grid(0, 2) == 3);
    REQUIRE(grid(2, 0) == 7);
    REQUIRE(grid(2, 2) == 9);
    REQUIRE(grid(3, 0) == 0);
    REQUIRE(grid(4, 2) == 0);
}

TEST_CASE("Remove columns from Array2D")
{
    std::vector<int32_t> values = {
        1, 2, 3, 4, 5,
        6, 7, 8, 9, 0,
        1, 2, 3, 4, 5,
        6, 7, 8, 9, 0
    };

    GridLib::Array2D<int32_t> grid(move(values), 4, 5);
    grid.resize(3, 3);
    REQUIRE(grid.rows() == 3);
    REQUIRE(grid.columns() == 3);
    REQUIRE(grid(0, 0) == 1);
    REQUIRE(grid(0, 2) == 3);
    REQUIRE(grid(1, 0) == 6);
    REQUIRE(grid(1, 2) == 8);
    REQUIRE(grid(2, 0) == 1);
    REQUIRE(grid(2, 2) == 3);
}

TEST_CASE("Resize empty Array2D")
{
    GridLib::Array2D<int32_t> grid;
    grid.resize(3, 3);
    REQUIRE(grid.rows() == 3);
    REQUIRE(grid.columns() == 3);
}

TEST_CASE("Iterate over an Array2D")
{
    std::vector<int32_t> values = {
        0, 2, 0,
        3, 4, 3,
        0, 2, 0
    };

    GridLib::Array2D<int32_t> grid(values, 3, 3);
    auto it = values.begin();
    for (auto row : grid)
    {
        for (auto value : row)
        {
            REQUIRE(value == *it++);
        }
    }
}
