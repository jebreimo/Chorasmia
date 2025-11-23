//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-14.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/ArrayView2DAlgorithms.hpp>
#include <Chorasmia/Array2D.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <Xyz/Vector.hpp>

using Catch::Matchers::WithinAbs;
constexpr auto MARGIN = Xyz::Constants<double>::DEFAULT_MARGIN;

TEST_CASE("Test copy")
{
    using namespace Chorasmia;
    Array2D<int> a({1, 2, 3, 4, 5, 6}, {2, 3});
    Array2D<int> b({3, 2});
    copy<int>(a.view(), b.mut(), Index2DMode::REVERSED_COLUMNS_REVERSED_ORDER);
    REQUIRE(b[{0, 0}] == 6);
    REQUIRE(b[{0, 1}] == 3);
    REQUIRE(b[{1, 0}] == 5);
    REQUIRE(b[{1, 1}] == 2);
    REQUIRE(b[{2, 0}] == 4);
    REQUIRE(b[{2, 1}] == 1);
}

TEST_CASE("Test that interpolate_value supports non-primitive types")
{
    using namespace Chorasmia;
    Array2D<Xyz::Vector2D> a(
        {
            {0, 1}, {1, 2}, {2, 3},
            {4, 5}, {5, 6}, {6, 7},
            {8, 9}, {9, 10}, {10, 11}
        }, {3, 3});
    REQUIRE(interpolate_value(a.view(), 0.5, 0.5) == Xyz::Vector2D(2.5, 3.5));
    REQUIRE(interpolate_value(a.view(), 0.5, 0) == Xyz::Vector2D(2, 3));
    REQUIRE(interpolate_value(a.view(), 2, 1.5) == Xyz::Vector2D(9.5, 10.5));
    REQUIRE(interpolate_value(a.view(), 2, 2) == Xyz::Vector2D(10, 11));
}

TEST_CASE("Test that interpolate_value behaves correctly on the boundary")
{
    using namespace Chorasmia;
    Array2D<double> a(
        {
            0, 1, 2,
            1, 2, 3,
            2, 3, 4
        }, {3, 3});
    REQUIRE(interpolate_value(a.view(), 0, 0) == 0);
    REQUIRE(interpolate_value(a.view(), 0, 0.5) == 0.5);
    REQUIRE(interpolate_value(a.view(), 0, 1) == 1);
    REQUIRE(interpolate_value(a.view(), 0, 1.5) == 1.5);
    REQUIRE(interpolate_value(a.view(), 0, 2) == 2);

    REQUIRE(interpolate_value(a.view(), 0.5, 0) == 0.5);
    REQUIRE(interpolate_value(a.view(), 0.5, 2) == 2.5);

    REQUIRE(interpolate_value(a.view(), 1, 0) == 1);
    REQUIRE(interpolate_value(a.view(), 1, 2) == 3);

    REQUIRE(interpolate_value(a.view(), 1.5, 0) == 1.5);
    REQUIRE(interpolate_value(a.view(), 1.5, 2) == 3.5);

    REQUIRE(interpolate_value(a.view(), 2, 0) == 2);
    REQUIRE(interpolate_value(a.view(), 2, 0.5) == 2.5);
    REQUIRE(interpolate_value(a.view(), 2, 1) == 3);
    REQUIRE(interpolate_value(a.view(), 2, 1.5) == 3.5);
    REQUIRE(interpolate_value(a.view(), 2, 2) == 4);
}

TEST_CASE("Test that interpolate_value throws when x or y is outside the array")
{
    using namespace Chorasmia;
    Array2D<double> a({1, 2, 3, 4, 5, 6}, {2, 3});
    REQUIRE_THROWS_AS(interpolate_value(a.view(), -0.1, 0), ChorasmiaException);
    REQUIRE_THROWS_AS(interpolate_value(a.view(), -2, 0), ChorasmiaException);
    REQUIRE_THROWS_AS(interpolate_value(a.view(), 1.1, 0), ChorasmiaException);
    REQUIRE_THROWS_AS(interpolate_value(a.view(), 0, -0.1), ChorasmiaException);
    REQUIRE_THROWS_AS(interpolate_value(a.view(), 0, -100), ChorasmiaException);
    REQUIRE_THROWS_AS(interpolate_value(a.view(), 0, 2.3), ChorasmiaException);
}

TEST_CASE("Test that interpolate_value interpolates correctly")
{
    using namespace Chorasmia;
    Array2D<double> a(
        {
            0, 1, 2,
            1, 2, 3,
            2, 3, 4
        }, {3, 3});
    REQUIRE_THAT(interpolate_value(a.view(), 0.2, 1.3), WithinAbs(1.5, MARGIN));
}
