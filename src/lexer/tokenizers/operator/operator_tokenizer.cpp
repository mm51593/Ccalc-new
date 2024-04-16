#include "operator_tokenizer.hpp"

#include <cstdint>
#include <optional>
#include <variant>

using Initial = OperatorTokenizer::Initial;
using Valid = OperatorTokenizer::Valid;
using Invalid = OperatorTokenizer::Invalid;

//////////////////////////////////////////

struct ValidChar {
  uint8_t c;
};

using Input = std::variant<ValidChar, std::monostate>;

namespace OperatorInput {
Input from(uint8_t raw_input) {
  bool is_valid = false;
  for (uint8_t c : VALID_CHARS) {
    if (raw_input == c) {
      is_valid = true;
      break;
    }
  }

  if (is_valid) {
    return ValidChar{.c = raw_input};
  } else {
    return std::monostate();
  }
}
} // namespace OperatorInput

//////////////////////////////////////////

OperatorTokenizer::State transition(Initial &state, ValidChar &input) {
  Valid ret;
  ret.text[ret.idx++] = input.c;
  return ret;
}
OperatorTokenizer::State transition(Valid &state, ValidChar &input) {
  if (state.idx < MAX_LENGTH) {
    state.text[state.idx++] = input.c;
    return state;
  } else {
    return Invalid{};
  }
}
OperatorTokenizer::State transition(auto &state, auto &input) {
  return Invalid{};
}

//////////////////////////////////////////

std::optional<Token> produce_result(Valid final_state) {
  for (auto &[t, o] : OP_MAPPINGS) {
    if (final_state.text == t) {
      return OperatorToken{o};
    }
  }
  return std::nullopt;
}

//////////////////////////////////////////

OperatorTokenizer::Result get_result(Initial old_state, Invalid new_state) {
  return OperatorTokenizer::Error{};
}
OperatorTokenizer::Result get_result(Invalid old_state, Invalid new_state) {
  return OperatorTokenizer::Error{};
}
OperatorTokenizer::Result get_result(Valid old_state, Invalid new_state) {
  if (auto res = produce_result(old_state); res.has_value()) {
    return OperatorTokenizer::Done{res.value()};
  } else {
    return OperatorTokenizer::Error{};
  }
}
OperatorTokenizer::Result get_result(auto old_state, auto new_state) {
  return OperatorTokenizer::Pending{};
}

//////////////////////////////////////////

OperatorTokenizer::Result OperatorTokenizer::step(uint8_t raw_input) {
  Input input = OperatorInput::from(raw_input);

  auto next_state = std::visit(
      [](auto real_current_state, auto real_input) {
        return transition(real_current_state, real_input);
      },
      this->state, input);

  auto res = std::visit(
      [next_state](auto real_old_state, auto real_new_state) {
        return get_result(real_old_state, real_new_state);
      },
      this->state, next_state);

  this->state = next_state;
  return res;
}
