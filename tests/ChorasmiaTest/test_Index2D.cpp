//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/Index2DIterator.hpp>
#include <catch2/catch.hpp>

TEST_CASE("next() always returns false for empty BasicIndex2DIterator")
{
    Chorasmia::BasicIndex2DIterator it(0, 0);
    REQUIRE_FALSE(it.next());
    REQUIRE_FALSE(it.next());
}

TEST_CASE("Test BasicIndex2DIterator")
{
    Chorasmia::BasicIndex2DIterator it(2, 3);
    auto idx = it.next();
    REQUIRE(idx);
    REQUIRE(*idx == Chorasmia::Index2D{0, 0});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(*idx == Chorasmia::Index2D{0, 1});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(*idx == Chorasmia::Index2D{0, 2});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(*idx == Chorasmia::Index2D{1, 0});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(*idx == Chorasmia::Index2D{1, 1});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(*idx == Chorasmia::Index2D{1, 2});
    REQUIRE_FALSE(it.next());
    REQUIRE_FALSE(it.next());
}

TEST_CASE("Test MatrixTraversal")
{
    using TD = Chorasmia::MatrixTraversal;
    REQUIRE(transpose(TD::LEFT_DOWN) == TD::DOWN_LEFT);
    REQUIRE(transpose(TD::DOWN_RIGHT) == TD::RIGHT_DOWN);
    REQUIRE(transpose(TD::UP_LEFT) == TD::LEFT_UP);

    REQUIRE(rotateLeft(TD::UP_LEFT, 1) == TD::LEFT_DOWN);
    REQUIRE(rotateLeft(TD::UP_LEFT, 5) == TD::LEFT_DOWN);
    REQUIRE(rotateLeft(TD::UP_LEFT, -1) == TD::RIGHT_UP);
}

TEST_CASE("Test Index2DIterator")
{
    using namespace Chorasmia;
    Index2DIterator it(2, 3, MatrixTraversal::DOWN_LEFT);
    auto idx = it.next();
    REQUIRE(idx);
    REQUIRE(idx == Index2D{0, 2});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(idx == Index2D{1, 2});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(idx == Index2D{0, 1});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(idx == Index2D{1, 1});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(idx == Index2D{0, 0});
    idx = it.next();
    REQUIRE(idx);
    REQUIRE(idx == Index2D{1, 0});
    REQUIRE_FALSE(it.next());
    REQUIRE_FALSE(it.next());
}
