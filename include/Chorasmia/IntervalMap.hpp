//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-03-14.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "Approx.hpp"

namespace Chorasmia
{
    template <typename Key, typename Value>
    class IntervalMap
    {
    public:
        using Map = std::vector<std::pair<double, Value>>;
        using Iterator = typename Map::const_iterator;

        explicit IntervalMap(Value defaultValue = {},
                             Key margin = 100 * std::numeric_limits<Key>::epsilon())
            : m_Map{{std::numeric_limits<Key>::lowest(),
                     std::move(defaultValue)},
                    {std::numeric_limits<Key>::max(), Value()}},
              m_Margin(margin)
        {}

        [[nodiscard]]
        const std::pair<Key, Value>* data() const
        {
            return m_Map.data();
        }

        [[nodiscard]]
        size_t size() const
        {
            return m_Map.size();
        }

        [[nodiscard]]
        Iterator find(Key key) const
        {
            return begin() + findImpl(key);
        }

        void insert(Key key, Value value)
        {
            auto pos = findImpl(key);
            if (m_Map[pos].first == Approx<Key>(key))
            {
                m_Map[pos].second = std::move(value);
                return;
            }

            m_Map.insert(m_Map.begin() + pos + 1, {key, std::move(value)});
        }

        void insert(Key from, Key to, Value value)
        {
            if (from > to)
                std::swap(from, to);
            else if (from == to)
                return;

            auto fromPos = findImpl(from);
            auto toPos = findImpl(to);
            auto n = toPos - fromPos;

            size_t insertPos = fromPos + 1;
            if (m_Map[fromPos].first == Approx<Key>(from, m_Margin))
            {
                ++n;
                insertPos = fromPos;
            }

            if (n != 0)
            {
                m_Map[toPos].first = to;
            }

            if (n >= 2)
            {
                m_Map[insertPos] = {from, std::move(value)};
            }

            if (n == 0)
            {
                m_Map.insert(m_Map.begin() + insertPos,
                             {{from, std::move(value)},
                              {to, m_Map[fromPos].second}});
            }
            else if (n == 1)
            {
                m_Map.insert(m_Map.begin() + insertPos, {from, std::move(value)});
            }
            else if (n > 2)
            {
                m_Map.erase(m_Map.begin() + insertPos + 1,
                            m_Map.begin() + toPos);
            }
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
        size_t findImpl(Key value) const
        {
            Approx<Key> cmpValue(value, m_Margin);
            size_t b = 0, e = m_Map.size();
            while (e - b > 1)
            {
                auto m = b + (e - b) / 2;
                if (m_Map[m].first <= cmpValue)
                    b = m;
                else
                    e = m;
            }
            return b;
        }

        std::vector<std::pair<Key, Value>> m_Map;
        Key m_Margin;
    };
}
