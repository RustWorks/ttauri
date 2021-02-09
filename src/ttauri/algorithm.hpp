// Copyright Take Vos 2019-2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "required.hpp"
#include "assert.hpp"
#include "cast.hpp"
#include <algorithm>
#include <tuple>
#include <cmath>
#include <iterator>

namespace tt {

/** Transform an input container to the output container.
 * @param input Input container.
 * @param operation A function to execute on each element in the input
 * @return Output container containing the transformed elements.
 */
template<typename T, typename U, typename F>
inline T transform(const U &input, F operation)
{
    T result = {};
    result.reserve(input.size());
    std::transform(input.begin(), input.end(), std::back_inserter(result), operation);
    return result;
}

/** Generate data in an array.
 * @param operation A function to execute for each element in the array.
 *                  The function accepts a single index argument.
 * @return An array filled with elements generated by operation.
 */
template<typename T, size_t N, typename F>
constexpr std::array<T, N> generate_array(F operation)
{
    std::array<T, N> a{};

    for (size_t i = 0; i < N; i++) {
        a.at(i) = operation(i);
    }

    return a;
}

/** Remove an element from an container.
 * @param v Container with elements that need to be removed.
 * @param predicate A function that is passed to std::find_if() to find elements that need to be erased.
 */
template<typename T, typename F>
inline void erase_if(T &v, F predicate)
{
    while (true) {
        ttlet i = std::find_if(v.begin(), v.end(), predicate);
        if (i == v.end()) {
            return;
        }
        v.erase(i);
    }
}

template<typename It, typename UnaryPredicate>
constexpr It rfind_if(It const first, It const last, UnaryPredicate predicate)
{
    auto i = last;
    do {
        i--;
        if (predicate(*i)) {
            return i;
        }
    } while (i != first);
    return last;
}

template<typename It, typename UnaryPredicate>
constexpr It rfind_if_not(It const first, It const last, UnaryPredicate predicate)
{
    return rfind_if(first, last, [&](ttlet &x) {
        return !predicate(x);
    });
}

template<typename It, typename T>
constexpr It rfind(It const first, It const last, T const &value)
{
    return rfind_if(first, last, [&](ttlet &x) {
        return x == value;
    });
}

/** Find the first occurrence of an value in a data.
 * @param data_first An iterator pointing to the first item of data.
 * @param data_last An iterator pointing one beyond the last item of data.
 * @param value_first An iterator pointing to a value to find in data.
 * @param value_last An iterator pointing on beyond the last value to find in data.
 * @return An iterator within data for the first matching value, or data_last if not found.
 */
template<typename It, typename ItAny>
[[nodiscard]] constexpr It find_any(It data_first, It data_last, ItAny value_first, ItAny value_last) noexcept
{
    return std::find_if(data_first, data_last, [value_first, value_last](ttlet &data) {
        return std::any_of(value_first, value_last, [&data](ttlet &value) {
            return data == value;
        });
    });
}

/** Find the start of the current cluster.
 * @param last The last iterator, where this function will stop iterating.
 * @param start Where to start the search
 * @param predicate A function returning the identifier of the cluster.
 * @return One beyond the last iterator where the cluster is the same as start.
 */
template<typename ConstIt, typename It, typename UnaryPredicate>
constexpr It find_cluster(ConstIt last, It start, UnaryPredicate predicate)
{
    ttlet cluster_id = predicate(*start);

    for (auto i = start + 1; i != last; ++i) {
        if (predicate(*i) != cluster_id) {
            return i;
        }
    }
    return last;
}

/** Find the start of the current cluster.
 * @param first The first iterator, where this function will stop iterating.
 * @param start Where to start the search
 * @param predicate A function returning the identifier of the cluster.
 * @return The first iterator where the cluster is the same as start.
 */
template<typename ConstIt, typename It, typename UnaryPredicate>
constexpr It rfind_cluster(ConstIt first, It start, UnaryPredicate predicate)
{
    ttlet cluster_id = predicate(*start);

    if (start == first) {
        return first;
    }

    auto i = start - 1;
    while (true) {
        if (predicate(*i) != cluster_id) {
            return (i + 1);
        }

        if (i == first) {
            return i;
        }
        --i;
    }
    tt_unreachable();
}

/** Find the begin and end of the current cluster.
 * @param first The first iterator, where this function will stop iterating.
 * @param last The last iterator, where this function will stop iterating.
 * @param start Where to start the search
 * @param predicate A function returning the identifier of the cluster.
 * @return The first and one beyond last iterator where the cluster is the same as start.
 */
template<typename ConstIt, typename It, typename UnaryPredicate>
constexpr std::pair<It, It> bifind_cluster(ConstIt first, ConstIt last, It start, UnaryPredicate predicate)
{
    return {rfind_cluster(first, start, predicate), find_cluster(last, start, predicate)};
}

/*! For each cluster.
 * func() is executed for each cluster that is found between first-last.
 * A cluster is found between two separators, a separator is detected with IsClusterSeperator().
 * A cluster does not include the separator itself.
 */
template<typename It, typename S, typename F>
inline void for_each_cluster(It first, It last, S IsClusterSeperator, F Function)
{
    if (first == last) {
        return;
    }

    // If the first item is a cluster seperator skip over it.
    if (IsClusterSeperator(*first)) {
        first++;
    }

    for (auto i = first; i != last;) {
        auto j = std::find_if(i, last, IsClusterSeperator);
        Function(i, j);

        auto skipOverSeperator = (j == last) ? 0 : 1;
        i = j + skipOverSeperator;
    }
}

template<typename InputIt1, typename InputIt2, typename BinaryPredicate>
inline std::pair<InputIt1, InputIt2>
rmismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate predicate) noexcept
{
    auto i1 = last1;
    auto i2 = last2;

    while (true) {
        if (i1 == first1 && i2 == first2) {
            return {last1, last2};
        } else if (i1 == first1) {
            return {last1, --i2};
        } else if (i2 == first2) {
            return {--i1, last2};
        }

        if (!predicate(*(--i1), *(--i2))) {
            return {i1, i2};
        }
    }
}

template<typename InputIt1, typename InputIt2>
inline std::pair<InputIt1, InputIt2> rmismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) noexcept
{
    return rmismatch(first1, last1, first2, last2, [&](auto a, auto b) {
        return a == b;
    });
}

template<typename T>
T smoothstep(T x) noexcept
{
    x = std::clamp(x, T{0.0}, T{1.0});
    return x * x * (3 - 2 * x);
}

template<typename T>
T inverse_smoothstep(T x)
{
    return T{0.5} - std::sin(std::asin(T{1.0} - T{2.0} * x) / T{3.0});
}

/** Mix between two values.
 * @param mix_value value between 0.0 and 1.0, the relative distance from lhs to rhs.
 * @param lhs The value when mix_value is 0.0
 * @param rhs The value when mix_value is 1.0
 */
template<typename T, typename MixType, std::enable_if_t<std::is_floating_point_v<MixType>, int> = 0>
T mix(MixType mix_value, T const &lhs, T const &rhs) noexcept
{
    if (mix_value >= MixType(1.0)) {
        return rhs;
    } else if (mix_value <= MixType(0.0)) {
        return lhs;
    } else {
        return lhs + (rhs - lhs) * mix_value;
    }
}

/** Shuffle a container based on a list of indices.
 * It is undefined behavior for an index to point beyond `last`.
 * It is undefined behavior for an index to repeat.
 *
 * Complexity is O(n) swaps, where n is the number of indices.
 *
 * @param first An iterator pointing to the first item in a container to be shuffled (index = 0)
 * @param last An iterator pointing beyond the last item in a container to be shuffled.
 * @param indices_first An iterator pointing to the first index.
 * @param indices_last An iterator pointing beyond the last index.
 * @param index_op A function returning the `size` index from indices.
 *                 The default returns the index item it self.
 * @return An iterator pointing beyond the last element that was added by the indices.
 *         first + std::distance(indices_first, indices_last)
 */
auto shuffle_by_index(auto first, auto last, auto indices_first, auto indices_last, auto index_op) noexcept
{
    size_t size = std::distance(first, last);

    // Keep track of index locations during shuffling of items.
    auto src_indices = std::vector<size_t>{};
    src_indices.reserve(size);
    for (size_t i = 0; i != size; ++i) {
        src_indices.push_back(i);
    }

    size_t dst = 0;
    for (auto it = indices_first; it != indices_last; ++it, ++dst) {
        ttlet index = index_op(*it);
        tt_axiom(index < std::size(src_indices));

        auto src = [&src_indices,index]() {
            auto src = index;
            do {
                src = src_indices[src];
            } while (src_indices[src] != index);
            return src;
        }();

        if (src != dst) {
            std::iter_swap(first + src, first + dst);
            std::iter_swap(std::begin(src_indices) + src, std::begin(src_indices) + dst);
        }
    }

    return first + dst;
}

/** Shuffle a container based on a list of indices.
 * It is undefined behavior for an index to point beyond `last`.
 * It is undefined behavior for an index to repeat.
 *
 * Complexity is O(n) swaps, where n is the number of indices.
 *
 * @param first An iterator pointing to the first item in a container to be shuffled (index = 0)
 * @param last An iterator pointing beyond the last item in a container to be shuffled.
 * @param indices_first An iterator pointing to the first index.
 * @param indices_last An iterator pointing beyond the last index.
 * @return An iterator pointing beyond the last element that was added by the indices.
 *         first + std::distance(indices_first, indices_last)
 */
auto shuffle_by_index(auto first, auto last, auto indices_first, auto indices_last) noexcept
{
    return shuffle_by_index(first, last, indices_first, indices_last, [](ttlet &x) {
        return narrow_cast<size_t>(x);
    });
}

/** Strip data from the front side.
 * @param data_first The iterator pointing to the first element of data.
 * @param data_last The iterator pointing one beyond the last element of data.
 * @param value_first The iterator pointing to the first value to be removed from data.
 * @param value_last The iterator pointing one beyond the last value to be removed from data.
 * @return An iterator pointing to the first data element not belonging to the values to be stripped.
 *         or data_last when all data elements have been stripped.
 */
template<typename DataIt, typename ValueIt>
DataIt front_strip(DataIt data_first, DataIt data_last, ValueIt value_first, ValueIt value_last) noexcept
{
    for (auto it = data_first; it != data_last; ++it) {
        ttlet &data = *it;
        if (!std::any_of(value_first, value_last, [&data](ttlet &value) {
                return data == value;
            })) {
            return it;
        }
    }

    return data_last;
}

/** Strip data from the back side.
 * @param data_first The iterator pointing to the first element of data.
 * @param data_last The iterator pointing one beyond the last element of data.
 * @param value_first The iterator pointing to the first value to be removed from data.
 * @param value_last The iterator pointing one beyond the last value to be removed from data.
 * @return An iterator pointing one beyond the first data element not belonging to the values to be stripped.
 *         or data_first when all data elements have been stripped.
 */
template<typename DataIt, typename ValueIt>
DataIt back_strip(DataIt data_first, DataIt data_last, ValueIt value_first, ValueIt value_last) noexcept
{
    for (auto it = data_last - 1; it >= data_first; ++it) {
        ttlet &data = *it;
        if (!std::any_of(value_first, value_last, [&data](ttlet &value) {
                return data == value;
            })) {
            return it + 1;
        }
    }

    return data_first;
}

} // namespace tt
