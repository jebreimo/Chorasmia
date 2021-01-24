//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <iterator>

namespace Ylgorithm
{
    template <typename RndIt, typename T>
    RndIt binaryFind(RndIt beg, RndIt end, const T& value)
    {
        RndIt it = std::lower_bound(beg, end, value);
        if (it == end || value < *it)
          return end;
        return it;
    }

    template <typename RndIt, typename T, typename UnaryFunc>
    RndIt lowerBound(RndIt beg, RndIt end, const T& value, UnaryFunc keyFunc)
    {
        while (beg != end)
        {
            RndIt mid = beg + std::distance(beg, end) / 2;
            if (keyFunc(*mid) < value)
                beg = mid + 1;
            else
                end = mid;
        }
        return beg;
    }

    template <typename RndIt, typename T, typename UnaryFunc>
    RndIt upperBound(RndIt beg, RndIt end, const T& value, UnaryFunc keyFunc)
    {
        while (beg != end)
        {
            RndIt mid = beg + std::distance(beg, end) / 2;
            if (value < keyFunc(*mid))
                end = mid;
            else
                beg = mid + 1;
        }
        return beg;
    }

    template <typename RndIt, typename T, typename UnaryFunc>
    RndIt binaryFind(RndIt beg, RndIt end, const T& value, UnaryFunc keyFunc)
    {
        RndIt it = lowerBound(beg, end, value, keyFunc);
        if (it == end || value < keyFunc(*it))
          return end;
        return it;
    }

    template <typename RndIt, typename T, typename UnaryFunc>
    bool binarySearch(RndIt beg, RndIt end, const T& value, UnaryFunc keyFunc)
    {
        RndIt it = lowerBound(beg, end, value, keyFunc);
        return it != end && value == keyFunc(*it);
    }

    template <typename RndIt, typename T, typename UnaryFunc>
    std::pair<RndIt, RndIt> bounds(RndIt beg, RndIt end, const T& value,
                                   UnaryFunc keyFunc)
    {
        RndIt first = lowerBound(beg, end, value, keyFunc);
        RndIt second = upperBound(first, end, value, keyFunc);
        return std::make_pair(first, second);
    }
}
