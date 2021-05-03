//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Chorasmia/Index2DMap.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Test MatrixTraversalMethod")
{
    using MP = Chorasmia::Index2DMode;
    REQUIRE(transpose(MP::REVERSED_ROWS) == MP::COLUMNS_REVERSED_ORDER);
    REQUIRE(transpose(MP::COLUMNS) == MP::ROWS);
    REQUIRE(transpose(MP::REVERSED_COLUMNS_REVERSED_ORDER) == MP::REVERSED_ROWS_REVERSED_ORDER);

    REQUIRE(invert(MP::ROWS) == MP::ROWS);
    REQUIRE(invert(MP::REVERSED_COLUMNS) == MP::COLUMNS_REVERSED_ORDER);
    REQUIRE(invert(MP::REVERSED_ROWS_REVERSED_ORDER) == MP::REVERSED_ROWS_REVERSED_ORDER);
    REQUIRE(invert(MP::COLUMNS_REVERSED_ORDER) == MP::REVERSED_COLUMNS);
    REQUIRE(invert(MP::COLUMNS) == MP::COLUMNS);
}

TEST_CASE("Test rotateLeft")
{
    using MP = Chorasmia::Index2DMode;
    REQUIRE(rotateLeft(MP::ROWS, 0) == MP::ROWS);
    REQUIRE(rotateLeft(MP::ROWS, 1) == MP::COLUMNS_REVERSED_ORDER);
    REQUIRE(rotateLeft(MP::ROWS, 2) == MP::REVERSED_ROWS_REVERSED_ORDER);
    REQUIRE(rotateLeft(MP::ROWS, 3) == MP::REVERSED_COLUMNS);
    REQUIRE(rotateLeft(MP::ROWS, 4) == MP::ROWS);
    REQUIRE(rotateLeft(MP::ROWS, 5) == MP::COLUMNS_REVERSED_ORDER);
    REQUIRE(rotateLeft(MP::ROWS, -1) == MP::REVERSED_COLUMNS);

    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, 0) == MP::REVERSED_COLUMNS_REVERSED_ORDER);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, 1) == MP::REVERSED_ROWS);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, 2) == MP::COLUMNS);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, 3) == MP::ROWS_REVERSED_ORDER);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, 4) == MP::REVERSED_COLUMNS_REVERSED_ORDER);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, 5) == MP::REVERSED_ROWS);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, -1) == MP::ROWS_REVERSED_ORDER);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, -2) == MP::COLUMNS);
    REQUIRE(rotateLeft(MP::REVERSED_COLUMNS_REVERSED_ORDER, -3) == MP::REVERSED_ROWS);
}

TEST_CASE("Test invert Index2DMode")
{
    using MP = Chorasmia::Index2DMode;

    REQUIRE(invert(MP::ROWS) == MP::ROWS);
    REQUIRE(invert(MP::REVERSED_COLUMNS) == MP::COLUMNS_REVERSED_ORDER);
    REQUIRE(invert(MP::REVERSED_ROWS_REVERSED_ORDER) == MP::REVERSED_ROWS_REVERSED_ORDER);
    REQUIRE(invert(MP::COLUMNS_REVERSED_ORDER) == MP::REVERSED_COLUMNS);

    REQUIRE(invert(MP::COLUMNS) == MP::COLUMNS);
    REQUIRE(invert(MP::REVERSED_ROWS_REVERSED_ORDER) == MP::REVERSED_ROWS_REVERSED_ORDER);
    REQUIRE(invert(MP::COLUMNS_REVERSED_ORDER) == MP::REVERSED_COLUMNS);
    REQUIRE(invert(MP::REVERSED_COLUMNS_REVERSED_ORDER) == MP::REVERSED_COLUMNS_REVERSED_ORDER);
}

namespace Catch
{
    template <>
    struct StringMaker<std::pair<size_t, size_t>>
    {
        static std::string convert(const std::pair<size_t, size_t>& p)
        {
            return "{" + std::to_string(p.first)
                   + ", " + std::to_string(p.second) + "}";
        }
    };
}

void test_Map(Chorasmia::Index2DMode p,
              size_t fromRows, size_t fromCols,
              const std::vector<std::pair<std::size_t, std::size_t>>& toIndices)
{
    using namespace Chorasmia;
    using id = std::pair<size_t, size_t>;
    Index2DMap map({fromRows, fromCols}, p);
    size_t n = 0;
    for (size_t i = 0; i < fromRows; ++i)
    {
        for (size_t j = 0; j < fromCols; ++j)
        {
            auto [iTo, jTo] = toIndices[n++];
            CAPTURE(p, i, j, iTo, jTo);
            auto to = map.getToIndices(i, j);
            REQUIRE(to == id(iTo, jTo));
            auto from = map.getFromIndices(iTo, jTo);
            REQUIRE(from == id(i, j));
        }
    }
}

TEST_CASE("Test Index2DIterator")
{
    using namespace Chorasmia;
    test_Map(Index2DMode::ROWS, 2, 3,
             {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}});
    test_Map(Index2DMode::REVERSED_ROWS, 2, 3,
             {{0, 2}, {0, 1}, {0, 0}, {1, 2}, {1, 1}, {1, 0}});
    test_Map(Index2DMode::ROWS_REVERSED_ORDER, 2, 3,
             {{1, 0}, {1, 1}, {1, 2}, {0, 0}, {0, 1}, {0, 2}});
    test_Map(Index2DMode::REVERSED_ROWS_REVERSED_ORDER, 2, 3,
             {{1, 2}, {1, 1}, {1, 0}, {0, 2}, {0, 1}, {0, 0}});
    test_Map(Index2DMode::COLUMNS, 2, 3,
             {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}});
    test_Map(Index2DMode::COLUMNS_REVERSED_ORDER, 2, 3,
             {{2, 0}, {1, 0}, {0, 0}, {2, 1}, {1, 1}, {0, 1}});
    test_Map(Index2DMode::REVERSED_COLUMNS, 2, 3,
             {{0, 1}, {1, 1}, {2, 1}, {0, 0}, {1, 0}, {2, 0}});
    test_Map(Index2DMode::REVERSED_COLUMNS_REVERSED_ORDER, 2, 3,
             {{2, 1}, {1, 1}, {0, 1}, {2, 0}, {1, 0}, {0, 0}});
}
