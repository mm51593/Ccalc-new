#pragma once

#include <string>
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

struct KeywordToken {
  enum class KeywordType {
    If,
    Else,
    Then,
    Begin,
    End,
    While,
    Func,
  };
  KeywordType type;
};

struct IdentifierToken {
  std::string name;
};

struct WhitespaceToken {};

using Token = std::variant<NumericToken, OperatorToken, KeywordToken, IdentifierToken, WhitespaceToken>;
