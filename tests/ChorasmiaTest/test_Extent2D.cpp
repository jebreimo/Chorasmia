//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-11-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/Extent2D.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Extent2D intersection")
{
    using namespace Chorasmia;

    Extent2D<int> a({2, 2}, {4, 4});
    Extent2D<int> b({4, 4}, {4, 4});
    auto c = get_intersection(a, b);
    REQUIRE(c.has_value());
    REQUIRE(c->origin == Index2D<int>{4, 4});
    REQUIRE(c->size == Size2D<int>{2, 2});

    b = Extent2D<int>({6, 6}, {4, 4});
    c = get_intersection(a, b);
    REQUIRE(!c.has_value());

    b = Extent2D<int>({0, 0}, {3, 3});
    c = get_intersection(a, b);
    REQUIRE(c.has_value());
    REQUIRE(c->origin == Index2D<int>{2, 2});
    REQUIRE(c->size == Size2D<int>{1, 1});

    b = Extent2D<int>({0, 0}, {2, 2});
    c = get_intersection(a, b);
    REQUIRE(!c.has_value());
}

TEST_CASE("Extent2D max handles overflow")
{
    using namespace Chorasmia;

    Extent2D<uint32_t> a({1000, 1000});
    REQUIRE(a.size == Size2D<uint32_t>::max());
    REQUIRE(a.max_index() == Index2D<uint32_t>::max());
    a.origin += Size2D<uint32_t>{500, 500};
    REQUIRE(a.max_index() == Index2D<uint32_t>::max());
}

TEST_CASE("Extent2D clamp")
{
    using namespace Chorasmia;
    Extent2D<int> a({20, 50}, {100, 200});
    REQUIRE(clamp(a, Size2D{1000, 1000}) == a);
    REQUIRE(clamp(a, Size2D{120, 250}) == a);
    REQUIRE(clamp(a, Size2D{119, 249}) == Extent2D<int>({20, 50}, {99, 199}));
    REQUIRE(clamp(a, Size2D{80, 90}) == Extent2D<int>({20, 50}, {60, 40}));
    REQUIRE(clamp(a, Size2D{10, 10}) == Extent2D<int>({10, 10}, {0, 0}));
}
