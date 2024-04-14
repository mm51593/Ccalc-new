#include "numeric_tokenizer.hpp"

#include <cctype>
#include <cstdint>
#include <variant>

#define RADIX 10

using Initial = NumericTokenizer::Initial;
using Whole = NumericTokenizer::Whole;
using Decimal = NumericTokenizer::Decimal;
using Invalid = NumericTokenizer::Invalid;

//////////////////////////////////////////

struct Digit {
  uint32_t x;
};
struct DecimalPoint {};

using Input = std::variant<Digit, DecimalPoint, std::monostate>;

Input from(uint8_t raw_input) {
  if (raw_input == '.') {
    return DecimalPoint{};
  } else if (isdigit(raw_input)) {
    uint32_t value = raw_input - '0';
    return Digit{.x = value};
  } else {
    return std::monostate();
  }
}

//////////////////////////////////////////

NumericTokenizer::State transition(Initial state, Digit input) {
  return Whole{.x = input.x};
}
NumericTokenizer::State transition(Initial state, DecimalPoint input) {
  return Decimal{};
}
NumericTokenizer::State transition(Whole state, Digit input) {
  state.x = state.x * RADIX + input.x;
  return state;
}
NumericTokenizer::State transition(Whole state, DecimalPoint input) {
  return Decimal{};
}
NumericTokenizer::State transition(Decimal state, Digit input) {
  state.x = state.x * RADIX + input.x;
  return state;
}
NumericTokenizer::State transition(auto state, auto input) { return Invalid{}; }

//////////////////////////////////////////

template <> void NumericTokenizer::update(Whole state) {
  this->whole = state.x;
}
template <> void NumericTokenizer::update(Decimal state) {
  this->decimal = state.x;
}
void NumericTokenizer::update(auto state) {}

//////////////////////////////////////////

NumericTokenizer::Result get_result(NumericTokenizer *tknzr, Initial old_state,
                                    Invalid new_state) {
  return NumericTokenizer::Error{};
}
NumericTokenizer::Result get_result(NumericTokenizer *tknzr, Invalid old_state,
                                    Invalid new_state) {
  return NumericTokenizer::Error{};
}
NumericTokenizer::Result get_result(NumericTokenizer *tknzr, auto old_state,
                                    Invalid new_state) {
  auto res = tknzr->produce_result();
  if (res.has_value()) {
    return NumericTokenizer::Done{res.value()};
  } else {
    return NumericTokenizer::Error{};
  }
}
NumericTokenizer::Result get_result(NumericTokenizer *tknzr, auto old_state,
                                    auto new_state) {
  return NumericTokenizer::Pending{};
}

//////////////////////////////////////////

std::optional<Token> NumericTokenizer::produce_result() {
  uint64_t base = RADIX;

  while (this->decimal > base) {
    base *= RADIX;
  }

  return NumericToken{.value = this->whole + (double)this->decimal / base};
}

NumericTokenizer::Result NumericTokenizer::step(uint8_t raw_input) {
  Input input = from(raw_input);

  auto next_state = std::visit(
      [this](auto real_current_state, auto real_input) {
        auto next_state = transition(real_current_state, real_input);
        std::visit(
            [this](auto real_next_state) { this->update(real_next_state); },
            next_state);
        return next_state;
      },
      this->state, input);

  auto res = std::visit(
      [this, next_state](auto old_state, auto new_state) {
        return get_result(this, old_state, new_state);
      },
      this->state, next_state);

  this->state = next_state;
  return res;
}
