#pragma once

#include "../tokenizer.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <variant>

#define MAX_LENGTH 2 
#define VALID_CHARS "+-*/=!<>"

static_assert(MAX_LENGTH > 0);

struct OpMapping {
  std::array<uint8_t, MAX_LENGTH + 1> text;
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
};

class OperatorTokenizer : public Tokenizer {
public:
  struct Initial {};
  struct Valid {
    std::array<uint8_t, MAX_LENGTH + 1> text = {0};
    uint8_t idx = 0;
  };
  struct Invalid {};

  using State = std::variant<Initial, Valid, Invalid>;

  State state = Initial{};

  Result step(uint8_t raw_input) override;

  std::optional<Token> produce_result() override;

  void update(auto state);

private:
  std::array<uint8_t, MAX_LENGTH + 1> op_text;
};
