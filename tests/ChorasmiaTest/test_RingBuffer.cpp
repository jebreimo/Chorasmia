//****************************************************************************
// Copyright © 2023 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2023-05-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/RingBuffer.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Something")
{
    Chorasmia::RingBuffer<int, 3> buffer;
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.empty());
    buffer.push(1);
    REQUIRE(buffer.size() == 1);
    REQUIRE(!buffer.empty());
    buffer.push(2);
    REQUIRE(buffer.size() == 2);
    buffer.push(3);
    REQUIRE(buffer.size() == 3);
    buffer.push(4);
    REQUIRE(buffer.size() == 3);
    buffer.push(5);
    REQUIRE(buffer.size() == 3);

    REQUIRE(buffer.front() == 3);
    REQUIRE(buffer.back() == 5);

    REQUIRE(buffer[0] == 3);
    REQUIRE(buffer[1] == 4);
    REQUIRE(buffer[2] == 5);

    REQUIRE(std::distance(buffer.begin(), buffer.end()) == 3);
}
