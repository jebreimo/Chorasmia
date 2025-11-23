//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-11-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Chorasmia/SaturationMath.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace Chorasmia;

template <std::signed_integral T>
void testSaturationAddition(const std::string& type_name)
{
    INFO(type_name);
    REQUIRE(saturating_add<T>(40, 50) == 90);
    REQUIRE(saturating_add<T>(40, -50) == -10);
    REQUIRE(saturating_add<T>(-40, 50) == 10);
    REQUIRE(saturating_add<T>(-40, -50) == -90);
    using NL = std::numeric_limits<T>;
    REQUIRE(saturating_add<T>(NL::max(), 50) == NL::max());
    REQUIRE(saturating_add<T>(NL::min(), -50) == NL::min());
    REQUIRE(saturating_add<T>(NL::max(), -50) == NL::max() - 50);
    REQUIRE(saturating_add<T>(NL::min(), 50) == NL::min() + 50);
}

template <std::unsigned_integral T>
void testSaturationAddition(const std::string& type_name)
{
    INFO(type_name);
    REQUIRE(saturating_add<T>(40, 50) == 90);
    using NL = std::numeric_limits<T>;
    REQUIRE(saturating_add<T>(NL::max(), 50) == NL::max());
    REQUIRE(saturating_add<T>(NL::min(), 50) == NL::min() + 50);
}

TEST_CASE("Saturating addition")
{
    testSaturationAddition<int8_t>("int8_t");
    testSaturationAddition<int16_t>("int16_t");
    testSaturationAddition<int32_t>("int32_t");
    testSaturationAddition<int64_t>("int64_t");
    testSaturationAddition<uint8_t>("uint8_t");
    testSaturationAddition<uint16_t>("uint16_t");
    testSaturationAddition<uint32_t>("uint32_t");
    testSaturationAddition<uint64_t>("uint64_t");
}

template <std::signed_integral T>
void testSaturationSubtraction(const std::string& type_name)
{
    INFO(type_name);
    REQUIRE(saturating_subtract<T>(50, 40) == 10);
    REQUIRE(saturating_subtract<T>(40, 50) == -10);
    REQUIRE(saturating_subtract<T>(-50, -40) == -10);
    REQUIRE(saturating_subtract<T>(-40, -50) == 10);
    using NL = std::numeric_limits<T>;
    REQUIRE(saturating_subtract<T>(NL::max(), -50) == NL::max());
    REQUIRE(saturating_subtract<T>(NL::min(), 50) == NL::min());
    REQUIRE(saturating_subtract<T>(NL::max(), 50) == NL::max() - 50);
    REQUIRE(saturating_subtract<T>(NL::min(), -50) == NL::min() + 50);
}

template <std::unsigned_integral T>
void testSaturationSubtraction(const std::string& type_name)
{
    INFO(type_name);
    REQUIRE(saturating_subtract<T>(50, 40) == 10);
    using NL = std::numeric_limits<T>;
    REQUIRE(saturating_subtract<T>(NL::min(), 50) == NL::min());
    REQUIRE(saturating_subtract<T>(NL::max(), 50) == NL::max() - 50);
}

TEST_CASE("Saturating subtraction")
{
    testSaturationSubtraction<int8_t>("int8_t");
    testSaturationSubtraction<int16_t>("int16_t");
    testSaturationSubtraction<int32_t>("int32_t");
    testSaturationSubtraction<int64_t>("int64_t");
    testSaturationSubtraction<uint8_t>("uint8_t");
    testSaturationSubtraction<uint16_t>("uint16_t");
    testSaturationSubtraction<uint32_t>("uint32_t");
    testSaturationSubtraction<uint64_t>("uint64_t");
}

template <std::signed_integral T>
void testSaturationMultiplication(const std::string& type_name)
{
    INFO(type_name);
    REQUIRE(saturating_multiply<T>(6, 7) == 42);
    REQUIRE(saturating_multiply<T>(-6, 7) == -42);
    REQUIRE(saturating_multiply<T>(6, -7) == -42);
    REQUIRE(saturating_multiply<T>(-6, -7) == 42);
    using NL = std::numeric_limits<T>;
    REQUIRE(saturating_multiply<T>(NL::max() / 2, 3) == NL::max());
    REQUIRE(saturating_multiply<T>(NL::min() / 2, 3) == NL::min());
}

template <std::unsigned_integral T>
void testSaturationMultiplication(const std::string& type_name)
{
    INFO(type_name);
    REQUIRE(saturating_multiply<T>(6, 7) == 42);
    using NL = std::numeric_limits<T>;
    REQUIRE(saturating_multiply<T>(NL::max() / 2, 3) == NL::max());
}

TEST_CASE("Saturating multiplication")
{
    testSaturationMultiplication<int8_t>("int8_t");
    testSaturationMultiplication<int16_t>("int16_t");
    testSaturationMultiplication<int32_t>("int32_t");
    testSaturationMultiplication<int64_t>("int64_t");
    testSaturationMultiplication<uint8_t>("uint8_t");
    testSaturationMultiplication<uint16_t>("uint16_t");
    testSaturationMultiplication<uint32_t>("uint32_t");
    testSaturationMultiplication<uint64_t>("uint64_t");
}

TEST_CASE("Saturating division")
{
    using NL = std::numeric_limits<int32_t>;
    REQUIRE(saturating_divide<int32_t>(42, 7) == 6);
    REQUIRE(saturating_divide<int32_t>(-42, 7) == -6);
    REQUIRE(saturating_divide<int32_t>(42, -7) == -6);
    REQUIRE(saturating_divide<int32_t>(-42, -7) == 6);
    REQUIRE(saturating_divide<int32_t>(5, 2) == 2);
    REQUIRE(saturating_divide<int32_t>(-5, 2) == -2);
    REQUIRE(saturating_divide<int32_t>(5, -2) == -2);
    REQUIRE(saturating_divide<int32_t>(-5, -2) == 2);
    REQUIRE(saturating_divide<int32_t>(1, 0) == NL::max());
    REQUIRE(saturating_divide<int32_t>(-1, 0) == NL::min());
    REQUIRE(saturating_divide<int32_t>(1, 0) == NL::max());
    REQUIRE(saturating_divide<int32_t>(-1, 0) == NL::min());
    REQUIRE(saturating_divide<int32_t>(NL::max(), -1) == -NL::max());
    REQUIRE(saturating_divide<int32_t>(NL::min(), -1) == NL::max());
}
