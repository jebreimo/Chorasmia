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
    template <typename T>
    constexpr T get_default_margin()
    {
        if constexpr (std::is_floating_point_v<T>)
            return 100 * std::numeric_limits<T>::epsilon();
        else
            return {};
    }

    template <typename Key, typename Value>
    class IntervalMap
    {
    public:
        using Map = std::vector<std::pair<Key, Value>>;
        using Iterator = typename Map::const_iterator;

        explicit IntervalMap(Value defaultValue = {},
                             Key margin = get_default_margin<Key>())
            : map_{{std::numeric_limits<Key>::lowest(),
                    std::move(defaultValue)},
                   {std::numeric_limits<Key>::max(), Value()}},
              margin_(margin)
        {}

        [[nodiscard]]
        const std::pair<Key, Value>* data() const
        {
            return map_.data();
        }

        [[nodiscard]]
        size_t size() const
        {
            return map_.size();
        }

        [[nodiscard]]
        Iterator find(Key key) const
        {
            return begin() + find_impl(key);
        }

        void insert(Key key, Value value)
        {
            auto pos = find_impl(key);
            if (are_equal(map_[pos].first, key))
            {
                map_[pos].second = std::move(value);
                return;
            }

            map_.insert(map_.begin() + pos + 1, {key, std::move(value)});
        }

        void insert(Key from, Key to, Value value)
        {
            if (from > to)
                std::swap(from, to);
            else if (from == to)
                return;

            auto from_pos = find_impl(from);
            auto to_pos = find_impl(to);
            auto n = to_pos - from_pos;

            size_t insertPos = from_pos + 1;
            if (are_equal(map_[from_pos].first, from))
            {
                ++n;
                insertPos = from_pos;
            }

            if (n != 0)
            {
                map_[to_pos].first = to;
            }

            if (n >= 2)
            {
                map_[insertPos] = {from, std::move(value)};
            }

            if (n == 0)
            {
                map_.insert(map_.begin() + insertPos,
                            {{from, std::move(value)},
                              {to, map_[from_pos].second}});
            }
            else if (n == 1)
            {
                map_.insert(map_.begin() + insertPos, {from, std::move(value)});
            }
            else if (n > 2)
            {
                map_.erase(map_.begin() + insertPos + 1,
                           map_.begin() + to_pos);
            }
        }

        [[nodiscard]]
        Iterator begin() const
        {
            return map_.begin();
        }

        [[nodiscard]]
        Iterator end() const
        {
            return map_.end();
        }
    private:
        [[nodiscard]]
        constexpr bool are_equal(Key a, Key b) const
        {
            if constexpr (std::is_floating_point_v<Key>)
                return std::abs(b - a) < margin_;
            else
                return a == b;
        }

        [[nodiscard]]
        constexpr bool is_less(Key a, Key b) const
        {
            if constexpr (std::is_floating_point_v<Key>)
                return b - a > margin_;
            else
                return a < b;
        }

        [[nodiscard]]
        size_t find_impl(Key value) const
        {
            size_t first = 0, last = map_.size();
            while (last - first > 1)
            {
                auto m = first + (last - first) / 2;
                if (is_less(value, map_[m].first))
                    last = m;
                else
                    first = m;
            }
            return first;
        }

        std::vector<std::pair<Key, Value>> map_;
        Key margin_;
    };
}
