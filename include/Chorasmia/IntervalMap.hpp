//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-14.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <limits>
#include <vector>

namespace Chorasmia
{
    template <typename Key, typename Value>
    class IntervalMap
    {
    public:
        using Map = std::vector<std::pair<double, Value>>;
        using Iterator = typename Map::const_iterator;

        explicit IntervalMap(Value defaultValue = {})
            : m_Map{{std::numeric_limits<Key>::lowest(),
                     std::move(defaultValue)}}
        {}

        [[nodiscard]]
        size_t size() const
        {
            return m_Map.size();
        }

        [[nodiscard]]
        Iterator find(double value) const
        {
            return begin() + findImpl(value);
        }

        Iterator insert(double key, Value value)
        {
            using std::move;
            if (m_Map.back().first < key)
            {
                m_Map.push_back({key, move(value)});
                return end() - 1;
            }

            auto pos = findImpl(key);
            if (m_Map[pos].first == key)
            {
                m_Map[pos].second = move(value);
                return begin() + pos;
            }

            return m_Map.insert(m_Map.begin() + pos + 1,
                                 {key, move(value)});
        }

        [[nodiscard]]
        Iterator begin() const
        {
            return m_Map.begin();
        }

        [[nodiscard]]
        Iterator end() const
        {
            return m_Map.end();
        }
    private:
        [[nodiscard]]
        size_t findImpl(double value) const
        {
            size_t b = 0, e = m_Map.size();
            while (e - b > 1)
            {
                auto m = b + (e - b) / 2;
                if (m_Map[m].first <= value)
                    b = m;
                else
                    e = b;
            }
            return b;
        }

        std::vector<std::pair<double, Value>> m_Map;
    };
}
