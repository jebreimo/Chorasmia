//****************************************************************************
// Copyright © 2023 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2023-05-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/RingBuffer.hpp>
#include <catch2/catch.hpp>

TEST_CASE("RingBuffer basics")
{
    Chorasmia::RingBuffer<int, 3> buffer;
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.empty());
    buffer.push_back(1);
    REQUIRE(buffer.size() == 1);
    REQUIRE(!buffer.empty());
    buffer.push_back(2);
    REQUIRE(buffer.size() == 2);
    buffer.push_back(3);
    REQUIRE(buffer.size() == 3);
    buffer.push_back(4);
    REQUIRE(buffer.size() == 3);
    buffer.push_back(5);
    REQUIRE(buffer.size() == 3);

    REQUIRE(buffer.front() == 3);
    REQUIRE(buffer.back() == 5);

    REQUIRE(buffer[0] == 3);
    REQUIRE(buffer[1] == 4);
    REQUIRE(buffer[2] == 5);

    REQUIRE(std::distance(buffer.begin(), buffer.end()) == 3);
}

TEST_CASE("Iterator arithmetic")
{
    Chorasmia::RingBuffer<int, 7> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4);
    buffer.push_back(5);
    buffer.push_back(6);
    buffer.push_back(7);
    // --
    buffer.push_back(8);
    buffer.push_back(9);
    buffer.push_back(10);

    REQUIRE(buffer.size() == 7);
    REQUIRE(buffer.front() == 4);
    REQUIRE(buffer.back() == 10);

    SECTION("Check subtraction")
    {
        REQUIRE(*(buffer.end() - 1) == 10);
        REQUIRE(*(buffer.end() - 2) == 9);
        REQUIRE(*(buffer.end() - 3) == 8);
        REQUIRE(*(buffer.end() - 4) == 7);
        REQUIRE(*(buffer.end() - 5) == 6);
        REQUIRE(*(buffer.end() - 6) == 5);
        REQUIRE(*(buffer.end() - 7) == 4);
    }

    SECTION("Check addition")
    {
        REQUIRE(*(buffer.begin() + 0) == 4);
        REQUIRE(*(buffer.begin() + 1) == 5);
        REQUIRE(*(buffer.begin() + 2) == 6);
        REQUIRE(*(buffer.begin() + 3) == 7);
        REQUIRE(*(buffer.begin() + 4) == 8);
        REQUIRE(*(buffer.begin() + 5) == 9);
        REQUIRE(*(buffer.begin() + 6) == 10);
    }

    SECTION("Check forward iteration")
    {
        auto it = buffer.begin();
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 6);
        REQUIRE(*it++ == 7);
        REQUIRE(*it++ == 8);
        REQUIRE(*it++ == 9);
        REQUIRE(*it++ == 10);
        REQUIRE(it == buffer.end());
    }

    SECTION("Check backward iteration")
    {
        auto it = buffer.end();
        REQUIRE(*--it == 10);
        REQUIRE(*--it == 9);
        REQUIRE(*--it == 8);
        REQUIRE(*--it == 7);
        REQUIRE(*--it == 6);
        REQUIRE(*--it == 5);
        REQUIRE(*--it == 4);
        REQUIRE(it == buffer.begin());
    }
}
