// Copyright Take Vos 2019-2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "ttauri/counters.hpp"
#include "ttauri/required.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace std;
using namespace tt;

TEST(Counters, CompileTime) {
    struct foo_a_tag {};
    struct bar_a_tag {};
    struct baz_a_tag {};

    increment_counter<foo_a_tag>();
    increment_counter<bar_a_tag>();
    increment_counter<bar_a_tag>();

    ASSERT_EQ(read_counter<baz_a_tag>(), 0);
    ASSERT_EQ(read_counter<foo_a_tag>(), 1);
    ASSERT_EQ(read_counter<bar_a_tag>(), 2);
}

TEST(Counters, RunTimeRead) {
    struct foo_b_tag {};
    struct bar_b_tag {};
    struct baz_b_tag {};

    increment_counter<foo_b_tag>();
    increment_counter<bar_b_tag>();
    increment_counter<bar_b_tag>();

    ASSERT_EQ(read_counter(std::type_index(typeid(baz_b_tag))).first, 0);
    ASSERT_EQ(read_counter(std::type_index(typeid(foo_b_tag))).first, 1);
    ASSERT_EQ(read_counter(std::type_index(typeid(bar_b_tag))).first, 2);
}

//TEST(Counters, RunTimeReadWithStrings) {
//    struct foo_c_tag {};
//    struct bar_c_tag {};
//    struct baz_c_tag {};
//
//    increment_counter<foo_c_tag>();
//    increment_counter<bar_c_tag>();
//    increment_counter<bar_c_tag>();
//
//    ASSERT_EQ(read_counter(std::type_index(typeid(baz_c_tag)).first, 0);
//    ASSERT_EQ(read_counter(std::type_index(typeid(foo_c_tag)).first, 1);
//    ASSERT_EQ(read_counter(std::type_index(typeid(bar_c_tag)).first, 2);
//}
