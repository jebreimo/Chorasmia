//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-24.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/Approx.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Approx comparisons")
{
    Chorasmia::Approx<double> a(1, 0.01);

    REQUIRE(a == 0.991);
    REQUIRE_FALSE(a == 0.989);

    REQUIRE(a != 0.989);
    REQUIRE_FALSE(a != 0.991);

    REQUIRE(a == 1.009);
    REQUIRE_FALSE(a == 1.011);

    REQUIRE(a != 1.011);
    REQUIRE_FALSE(a != 1.009);

    REQUIRE(a > 0.989);
    REQUIRE_FALSE(a > 0.991);

    REQUIRE(1.011 > a);
    REQUIRE_FALSE(1.009 > a);

    REQUIRE(a < 1.011);
    REQUIRE_FALSE(a < 1.009);

    REQUIRE(0.989 < a);
    REQUIRE_FALSE(0.991 < a);

    REQUIRE(a >= 1.009);
    REQUIRE_FALSE(a >= 1.011);

    REQUIRE(0.991 >= a);
    REQUIRE_FALSE(0.989 >= a);

    REQUIRE(1.009 <= a);
    REQUIRE_FALSE(1.011 <= a);

    REQUIRE(a <= 0.991);
    REQUIRE_FALSE(a <= 0.989);
}
