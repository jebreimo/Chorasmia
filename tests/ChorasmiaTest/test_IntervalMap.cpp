//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-14.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/IntervalMap.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Test IntervalMap")
{
    Chorasmia::IntervalMap<double, uint32_t> map;
    REQUIRE(map.size() == 1);
    REQUIRE(map.find(100)->second == 0);
    REQUIRE(map.insert(0, 1)->second == 1);
    REQUIRE(map.size() == 2);
    REQUIRE(map.find(100)->second == 1);
    REQUIRE(map.insert(200, 2)->second == 2);
    REQUIRE(map.find(100)->second == 1);
    REQUIRE(map.insert(50, 3)->second == 3);
    REQUIRE(map.find(100)->second == 3);
    REQUIRE(map.insert(100, 4)->second == 4);
    REQUIRE(map.find(100)->second == 4);
}
