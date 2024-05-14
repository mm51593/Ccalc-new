#pragma once

#include "../tokenizer.hpp"

#include <array>
#include <cstdint>
#include <variant>

const std::array<uint8_t, 10> VALID_CHARS = {'+', '-', '*', '/', '=',
                                             '!', '<', '>', '(', ')'};

const uint8_t OP_MAX_LENGTH = 2;
static_assert(OP_MAX_LENGTH > 0);

struct OpMapping {
  std::array<uint8_t, OP_MAX_LENGTH + 1> text;
  OperatorToken::OperatorType type;
};

const OpMapping OP_MAPPINGS[] = {
    {"+", OperatorToken::OperatorType::Add},
    {"-", OperatorToken::OperatorType::Subtract},
    {"*", OperatorToken::OperatorType::Multiply},
    {"/", OperatorToken::OperatorType::Divide},
    {"=", OperatorToken::OperatorType::Assign},
    {"==", OperatorToken::OperatorType::EQ},
    {"!=", OperatorToken::OperatorType::NE},
    {"<", OperatorToken::OperatorType::LT},
    {">", OperatorToken::OperatorType::GT},
    {"<=", OperatorToken::OperatorType::LTE},
    {">=", OperatorToken::OperatorType::GTE},
    {"(", OperatorToken::OperatorType::LParen},
    {")", OperatorToken::OperatorType::RParen},
};

class OperatorTokenizer : public Tokenizer {
public:
  struct Initial {};
  struct Valid {
    std::array<uint8_t, OP_MAX_LENGTH + 1> text = {0};
    uint8_t idx = 0;
  };
  struct Invalid {};

  using State = std::variant<Initial, Valid, Invalid>;

  State state = Initial{};

  Result step(uint8_t raw_input) override;
};
