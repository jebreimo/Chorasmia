//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-14.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/IntervalMap.hpp>
#include <catch2/catch.hpp>
#include <cfloat>

TEST_CASE("Test IntervalMap point insert")
{
    using P = std::pair<double, uint32_t>;
    Chorasmia::IntervalMap<double, uint32_t> map;
    REQUIRE(map.size() == 2);
    REQUIRE(map.find(100)->second == 0);
    map.insert(0, 1);
    REQUIRE(map.size() == 3);
    REQUIRE(map.data()[1] == P{0, 1});
    REQUIRE(map.find(100)->second == 1);
    map.insert(200, 2);
    REQUIRE(map.size() == 4);
    REQUIRE(map.data()[2] == P{200, 2});
    REQUIRE(map.find(100)->second == 1);
    map.insert(50, 3);
    REQUIRE(map.size() == 5);
    REQUIRE(map.data()[2] == P{50, 3});
    REQUIRE(map.find(100)->second == 3);
    map.insert(100, 4);
    REQUIRE(map.size() == 6);
    REQUIRE(map.data()[3] == P{100, 4});
    REQUIRE(map.find(100)->second == 4);
}

TEST_CASE("Test IntervalMap range insert")
{
    using P = std::pair<double, uint32_t>;
    Chorasmia::IntervalMap<double, uint32_t> map;
    REQUIRE(map.size() == 2);
    map.insert(0, 1, 1);
    REQUIRE(map.size() == 4);
    REQUIRE(map.data()[1] == P{0, 1});
    REQUIRE(map.data()[2] == P{1, 0});
    map.insert(1, 2, 2);
    REQUIRE(map.size() == 5);
    REQUIRE(map.data()[1] == P{0, 1});
    REQUIRE(map.data()[2] == P{1, 2});
    REQUIRE(map.data()[3] == P{2, 0});
    map.insert(2, 3, 3);
    REQUIRE(map.size() == 6);
    REQUIRE(map.data()[3] == P{2, 3});
    REQUIRE(map.data()[4] == P{3, 0});
    SECTION("Replace none")
    {
        map.insert(0.25, 0.5, 4);
        REQUIRE(map.size() == 8);
        REQUIRE(map.data()[1] == P{0, 1});
        REQUIRE(map.data()[2] == P{0.25, 4});
        REQUIRE(map.data()[3] == P{0.5, 1});
    }
    SECTION("Replace 1")
    {
        map.insert(0.5, 1.5, 4);
        REQUIRE(map.size() == 7);
        REQUIRE(map.data()[1] == P{0, 1});
        REQUIRE(map.data()[2] == P{0.5, 4});
        REQUIRE(map.data()[3] == P{1.5, 2});
    }
    SECTION("Replace 2")
    {
        map.insert(0.5, 2.5, 4);
        REQUIRE(map.size() == 6);
        REQUIRE(map.data()[1] == P{0, 1});
        REQUIRE(map.data()[2] == P{0.5, 4});
        REQUIRE(map.data()[3] == P{2.5, 3});
    }
    SECTION("Replace 2, start and end exists")
    {
        map.insert(1, 2, 4);
        REQUIRE(map.size() == 6);
        REQUIRE(map.data()[1] == P{0, 1});
        REQUIRE(map.data()[2] == P{1, 4});
        REQUIRE(map.data()[3] == P{2, 3});
    }
    SECTION("Replace 2, remove 1")
    {
        map.insert(0.5, 3.5, 4);
        REQUIRE(map.size() == 5);
        REQUIRE(map.data()[1] == P{0, 1});
        REQUIRE(map.data()[2] == P{0.5, 4});
        REQUIRE(map.data()[3] == P{3.5, 0});
    }
    SECTION("Replace 2, remove 2, start and end exists")
    {
        map.insert(0, 3, 4);
        REQUIRE(map.size() == 4);
        REQUIRE(map.data()[0] == P{-DBL_MAX, 0});
        REQUIRE(map.data()[1] == P{0, 4});
        REQUIRE(map.data()[2] == P{3, 0});
    }
    SECTION("Replace 1, start exists")
    {
        map.insert(1, 1.5, 4);
        REQUIRE(map.size() == 7);
        REQUIRE(map.data()[2] == P{1, 4});
        REQUIRE(map.data()[3] == P{1.5, 2});
        REQUIRE(map.data()[4] == P{2, 3});
    }
    SECTION("Replace 1, end exists")
    {
        map.insert(1.5, 2, 4);
        REQUIRE(map.size() == 7);
        REQUIRE(map.data()[2] == P{1, 2});
        REQUIRE(map.data()[3] == P{1.5, 4});
        REQUIRE(map.data()[4] == P{2, 3});
    }
}
