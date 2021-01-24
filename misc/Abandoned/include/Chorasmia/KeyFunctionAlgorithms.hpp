/* JEBBase: generic algorithms and functions
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#ifndef JEBBASE_ALGORITHMS_KEYFUNCTIONALGORITHMS_HPP
#define JEBBASE_ALGORITHMS_KEYFUNCTIONALGORITHMS_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>
#include "Algorithms.hpp"

/** @file
  * @brief Defines various wrappers and extensions to the generic algorithms
  *     in the standard library's algorithm header.
  */

namespace JEBBase
{
/** @brief The namespace for generic algorithms.
  */
namespace Algorithms
{

template <typename UnaryFunc>
class KeyComparer
{
public:
    KeyComparer(UnaryFunc keyFunc) : m_KeyFunc(keyFunc) {}
    template <typename T>
    bool operator()(T a, T b) {return m_KeyFunc(a) < m_KeyFunc(b);}
private:
    UnaryFunc m_KeyFunc;
};

template <typename RndIt, typename UnaryFunc>
void sortByKey(RndIt beg, RndIt end, UnaryFunc keyFunc)
{
    std::sort(beg, end, KeyComparer<UnaryFunc>(keyFunc));
}

template <typename FwdIt, typename UnaryFunc>
FwdIt maxElementByKey(FwdIt begin, FwdIt end, UnaryFunc keyFunc)
{
    return std::max_element(begin, end, KeyComparer<UnaryFunc>(keyFunc));
}

template <typename FwdIt, typename UnaryFunc>
FwdIt minElementByKey(FwdIt begin, FwdIt end, UnaryFunc keyFunc)
{
    return std::min_element(begin, end, KeyComparer<UnaryFunc>(keyFunc));
}

template <typename FwdIt, typename UnaryFunc>
std::pair<FwdIt, FwdIt> minmaxElementByKey(
        FwdIt begin, FwdIt end, UnaryFunc keyFunc)
{
    return std::minmax_element(begin, end, KeyComparer<UnaryFunc>(keyFunc));
}

}}

#endif
