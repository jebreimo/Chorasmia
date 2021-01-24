/* JEBBase: generic algorithms and functions
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#ifndef JEBBASE_FUNCTIONAL_PAIRS_HPP
#define JEBBASE_FUNCTIONAL_PAIRS_HPP

#include <utility>

namespace JEBBase { namespace Functional {

template <typename T>
std::vector<T> pairToVector(const std::pair<T, T>& p)
{
    std::vector<T> result;
    result.reserve(2);
    result.push_back(p.first);
    result.push_back(p.second);
    return result;
}

template <typename T>
std::vector<T> pairToVector(const std::pair<T, T>&& p)
{
    std::vector<T> result;
    result.reserve(2);
    result.push_back(std::move(p.first));
    result.push_back(std::move(p.second));
    return result;
}

}}

#endif
