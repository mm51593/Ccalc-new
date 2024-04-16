#include "numeric_tokenizer.hpp"

#include <cctype>
#include <cstdint>
#include <variant>
#include <optional>
#include <cmath>

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

namespace NumericInput {
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
}

//////////////////////////////////////////

NumericTokenizer::State transition(Initial &state, Digit input) {
  return Whole{.w = input.x};
}
NumericTokenizer::State transition(Initial &state, DecimalPoint input) {
  return Decimal{};
}
NumericTokenizer::State transition(Whole &state, Digit input) {
  state.w = state.w * RADIX + input.x;
  return state;
}
NumericTokenizer::State transition(Whole &state, DecimalPoint input) {
  return Decimal{ .w = state.w};
}
NumericTokenizer::State transition(Decimal &state, Digit input) {
  state.d = state.d * RADIX + input.x;
  state.count++;
  return state;
}
NumericTokenizer::State transition(auto &state, auto input) { return Invalid{}; }

//////////////////////////////////////////

std::optional<Token> produce_result(Whole final_state) {
  return NumericToken{.value = (double) final_state.w};
}
std::optional<Token> produce_result(Decimal final_state) {
  uint64_t base = pow(RADIX, final_state.count);

  return NumericToken{.value = final_state.w + (double)final_state.d / base};
}

//////////////////////////////////////////

NumericTokenizer::Result get_result(Initial old_state,
                                    Invalid new_state) {
  return NumericTokenizer::Error{};
}
NumericTokenizer::Result get_result(Invalid old_state,
                                    Invalid new_state) {
  return NumericTokenizer::Error{};
}
NumericTokenizer::Result get_result(auto old_state,
                                    Invalid new_state) {
  auto res = produce_result(old_state);
  if (res.has_value()) {
    return NumericTokenizer::Done{res.value()};
  } else {
    return NumericTokenizer::Error{};
  }
}
NumericTokenizer::Result get_result(auto old_state,
                                    auto new_state) {
  return NumericTokenizer::Pending{};
}

//////////////////////////////////////////

NumericTokenizer::Result NumericTokenizer::step(uint8_t raw_input) {
  Input input = NumericInput::from(raw_input);

  auto next_state = std::visit(
      [this](auto real_current_state, auto real_input) {
        auto next_state = transition(real_current_state, real_input);
        return next_state;
      },
      this->state, input);

  auto res = std::visit(
      [this, next_state](auto old_state, auto new_state) {
        return get_result(old_state, new_state);
      },
      this->state, next_state);

  this->state = next_state;
  return res;
}
