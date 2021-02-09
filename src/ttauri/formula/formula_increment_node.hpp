// Copyright Take Vos 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "formula_unary_operator_node.hpp"

namespace tt {

struct formula_increment_node final : formula_unary_operator_node {
    formula_increment_node(parse_location location, std::unique_ptr<formula_node> rhs) :
        formula_unary_operator_node(std::move(location), std::move(rhs)) {}

    datum evaluate(formula_evaluation_context& context) const override {
        auto &rhs_ = rhs->evaluate_lvalue(context);
        try {
            return ++rhs_;
        } catch (...) {
            error_info(true).set<parse_location_tag>(location);
            throw;
        }
    }

    std::string string() const noexcept override {
        return fmt::format("(++ {})", *rhs);
    }
};

}
