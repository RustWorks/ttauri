// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "ASTExpression.hpp"

namespace TTauri::Config {

struct ASTUnaryOperator : ASTExpression {
    enum class Operator {
        NOT,
        NEG,
        LOGICAL_NOT
    };

    Operator op;
    ASTExpression *right;

    ASTUnaryOperator(Location location, Operator op, ASTExpression *right) noexcept : ASTExpression(location), op(op), right(right) {}

    ~ASTUnaryOperator() {
        delete right;
    }

    std::string string() const noexcept override {
        std::string s;

        switch (op) {
        case Operator::NOT: s = "~"; break;
        case Operator::NEG: s = "-"; break;
        case Operator::LOGICAL_NOT: s = "not "; break;
        }

        return s + right->string();
    }

    universal_value execute(ExecutionContext &context) const override {
        try {
            switch (op) {
            case Operator::NOT: return ~right->execute(context);
            case Operator::NEG: return -right->execute(context);
            case Operator::LOGICAL_NOT: return !right->execute(context);
            }
            abort(); // Compiler doesn't recognize that switch is complete.
        } catch (error &e) {
            e << error_info("location", location);
            throw;
        }
    }
};

}
