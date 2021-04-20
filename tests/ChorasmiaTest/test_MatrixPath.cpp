//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/MatrixPath.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Test MatrixTraversalMethod")
{
    using MP = Chorasmia::MatrixPath;
    REQUIRE(transpose(MP::LEFT_DOWN) == MP::DOWN_LEFT);
    REQUIRE(transpose(MP::DOWN_RIGHT) == MP::RIGHT_DOWN);
    REQUIRE(transpose(MP::UP_LEFT) == MP::LEFT_UP);

    REQUIRE(rotateLeft(MP::UP_LEFT, 1) == MP::LEFT_DOWN);
    REQUIRE(rotateLeft(MP::UP_LEFT, 5) == MP::LEFT_DOWN);
    REQUIRE(rotateLeft(MP::UP_LEFT, -1) == MP::RIGHT_UP);

    REQUIRE(invert(MP::RIGHT_DOWN) == MP::RIGHT_DOWN);
    REQUIRE(invert(MP::UP_RIGHT) == MP::DOWN_LEFT);
    REQUIRE(invert(MP::LEFT_UP) == MP::LEFT_UP);
    REQUIRE(invert(MP::DOWN_LEFT) == MP::UP_RIGHT);

    REQUIRE(invert(MP::DOWN_RIGHT) == MP::DOWN_RIGHT);
}

TEST_CASE("Test Index2DIterator")
{
    using namespace Chorasmia;
    using id = std::pair<size_t, size_t>;
    REQUIRE(getFromIndex({4, 5}, 1, 3, MatrixPath::UP_RIGHT) == id(1, 1));
    REQUIRE(getFromIndex({4, 5}, 1, 3, MatrixPath::UP_LEFT) == id(1, 2));
 }
