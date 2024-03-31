//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Chorasmia/BitmaskOperators.hpp"
#include <catch2/catch_test_macros.hpp>

namespace Chorasmia
{
    enum class MyEnum
    {
        A = 1,
        B = 2,
        C = 4,
        D = 8
    };

    CHORASMIA_ENABLE_BITMASK_OPERATORS(Chorasmia::MyEnum);
}


TEST_CASE("Test operators")
{
    using namespace Chorasmia;
    REQUIRE(int(MyEnum::A | MyEnum::B) == 3);
    MyEnum e = MyEnum::A ^ MyEnum::B;
    REQUIRE(int(e) == 3);
    e |= MyEnum::C;
    REQUIRE(int(e) == 7);
    e ^= MyEnum::B;
    REQUIRE(int(e) == 5);
    e &= MyEnum::A | MyEnum::C | MyEnum::D;
    REQUIRE(int(e) == 5);
}
