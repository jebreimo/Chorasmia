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

    REQUIRE(invert(MP::RIGHT_DOWN) == MP::RIGHT_DOWN);
    REQUIRE(invert(MP::UP_RIGHT) == MP::DOWN_LEFT);
    REQUIRE(invert(MP::LEFT_UP) == MP::LEFT_UP);
    REQUIRE(invert(MP::DOWN_LEFT) == MP::UP_RIGHT);
    REQUIRE(invert(MP::DOWN_RIGHT) == MP::DOWN_RIGHT);
}

TEST_CASE("Test rotateLeft")
{
    using MP = Chorasmia::MatrixPath;
    REQUIRE(rotateLeft(MP::RIGHT_DOWN, 0) == MP::RIGHT_DOWN);
    REQUIRE(rotateLeft(MP::RIGHT_DOWN, 1) == MP::DOWN_LEFT);
    REQUIRE(rotateLeft(MP::RIGHT_DOWN, 2) == MP::LEFT_UP);
    REQUIRE(rotateLeft(MP::RIGHT_DOWN, 3) == MP::UP_RIGHT);
    REQUIRE(rotateLeft(MP::RIGHT_DOWN, 4) == MP::RIGHT_DOWN);
    REQUIRE(rotateLeft(MP::RIGHT_DOWN, 5) == MP::DOWN_LEFT);
    REQUIRE(rotateLeft(MP::RIGHT_DOWN, -1) == MP::UP_RIGHT);

    REQUIRE(rotateLeft(MP::UP_LEFT, 0) == MP::UP_LEFT);
    REQUIRE(rotateLeft(MP::UP_LEFT, 1) == MP::LEFT_DOWN);
    REQUIRE(rotateLeft(MP::UP_LEFT, 2) == MP::DOWN_RIGHT);
    REQUIRE(rotateLeft(MP::UP_LEFT, 3) == MP::RIGHT_UP);
    REQUIRE(rotateLeft(MP::UP_LEFT, 4) == MP::UP_LEFT);
    REQUIRE(rotateLeft(MP::UP_LEFT, 5) == MP::LEFT_DOWN);
    REQUIRE(rotateLeft(MP::UP_LEFT, -1) == MP::RIGHT_UP);
    REQUIRE(rotateLeft(MP::UP_LEFT, -2) == MP::DOWN_RIGHT);
    REQUIRE(rotateLeft(MP::UP_LEFT, -3) == MP::LEFT_DOWN);
}

TEST_CASE("Test invert MatrixPath")
{
    using MP = Chorasmia::MatrixPath;

    REQUIRE(invert(MP::RIGHT_DOWN) == MP::RIGHT_DOWN);
    REQUIRE(invert(MP::UP_RIGHT) == MP::DOWN_LEFT);
    REQUIRE(invert(MP::LEFT_UP) == MP::LEFT_UP);
    REQUIRE(invert(MP::DOWN_LEFT) == MP::UP_RIGHT);

    REQUIRE(invert(MP::DOWN_RIGHT) == MP::DOWN_RIGHT);
    REQUIRE(invert(MP::LEFT_UP) == MP::LEFT_UP);
    REQUIRE(invert(MP::DOWN_LEFT) == MP::UP_RIGHT);
    REQUIRE(invert(MP::UP_LEFT) == MP::UP_LEFT);
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

void test_Mapping(Chorasmia::MatrixPath p,
                  size_t fromRows, size_t fromCols,
                  const std::vector<std::pair<std::size_t, std::size_t>>& toIndices)
{
    using namespace Chorasmia;
    using id = std::pair<size_t, size_t>;
    MatrixIndexMapping mapping({fromRows, fromCols}, p);
    size_t n = 0;
    for (size_t i = 0; i < fromRows; ++i)
    {
        for (size_t j = 0; j < fromCols; ++j)
        {
            auto [iTo, jTo] = toIndices[n++];
            CAPTURE(p, i, j, iTo, jTo);
            auto to = mapping.getToIndices(i, j);
            REQUIRE(to == id(iTo, jTo));
            auto from = mapping.getFromIndices(iTo, jTo);
            REQUIRE(from == id(i, j));
        }
    }
}

TEST_CASE("Test Index2DIterator")
{
    using namespace Chorasmia;
    test_Mapping(MatrixPath::RIGHT_DOWN, 2, 3,
                 {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}});
    test_Mapping(MatrixPath::LEFT_DOWN, 2, 3,
                 {{0, 2}, {0, 1}, {0, 0}, {1, 2}, {1, 1}, {1, 0}});
    test_Mapping(MatrixPath::RIGHT_UP, 2, 3,
                 {{1, 0}, {1, 1}, {1, 2}, {0, 0}, {0, 1}, {0, 2}});
    test_Mapping(MatrixPath::LEFT_UP, 2, 3,
                 {{1, 2}, {1, 1}, {1, 0}, {0, 2}, {0, 1}, {0, 0}});
    test_Mapping(MatrixPath::DOWN_RIGHT, 2, 3,
                 {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}});
    test_Mapping(MatrixPath::DOWN_LEFT, 2, 3,
                 {{2, 0}, {1, 0}, {0, 0}, {2, 1}, {1, 1}, {0, 1}});
    test_Mapping(MatrixPath::UP_RIGHT, 2, 3,
                 {{0, 1}, {1, 1}, {2, 1}, {0, 0}, {1, 0}, {2, 0}});
    test_Mapping(MatrixPath::UP_LEFT, 2, 3,
                 {{2, 1}, {1, 1}, {0, 1}, {2, 0}, {1, 0}, {0, 0}});
}
