#pragma once

#include <variant>

struct NumericToken {
  double value = 0;
};

struct OperatorToken {
  enum class OperatorType {
    Add,
    Subtract,
    Multiply,
    Divide,
    Assign,
    EQ,
    NE,
    LT,
    GT,
    LTE,
    GTE
  };
  OperatorType type;
};

using Token = std::variant<NumericToken, OperatorToken>;
