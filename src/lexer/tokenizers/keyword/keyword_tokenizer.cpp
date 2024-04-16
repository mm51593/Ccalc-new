#include "keyword_tokenizer.hpp"

#include <cstdint>
#include <optional>
#include <variant>

using Initial = KeywordTokenizer::Initial;
using Valid = KeywordTokenizer::Valid;
using Invalid = KeywordTokenizer::Invalid;

//////////////////////////////////////////

struct ValidChar {
  uint8_t c;
};

using Input = std::variant<ValidChar, std::monostate>;

namespace KeywordInput {
Input from(uint8_t raw_input) {
  bool is_valid = raw_input >= 'a' && raw_input <= 'z';

  if (is_valid) {
    return ValidChar{.c = raw_input};
  } else {
    return std::monostate();
  }
}
}; // namespace KeywordInput

//////////////////////////////////////////

KeywordTokenizer::State transition(Initial &state, ValidChar &input) {
  Valid ret;
  ret.text[ret.idx++] = input.c;
  return ret;
}
KeywordTokenizer::State transition(Valid &state, ValidChar &input) {
  if (state.idx < KW_MAX_LENGTH) {
    state.text[state.idx++] = input.c;
    return state;
  } else {
    return Invalid{};
  }
}
KeywordTokenizer::State transition(auto &state, auto &input) {
  return Invalid{};
}

//////////////////////////////////////////

std::optional<Token> produce_result(Valid final_state) {
  for (auto &[t, k] : KW_MAPPINGS) {
    if (final_state.text == t) {
      return KeywordToken{k};
    }
  }
  return std::nullopt;
}

//////////////////////////////////////////

KeywordTokenizer::Result get_result(Initial old_state, Invalid new_state) {
  return KeywordTokenizer::Error{};
}
KeywordTokenizer::Result get_result(Invalid old_state, Invalid new_state) {
  return KeywordTokenizer::Error{};
}
KeywordTokenizer::Result get_result(Valid old_state, Invalid new_state) {
  if (auto res = produce_result(old_state); res.has_value()) {
    return KeywordTokenizer::Done{res.value()};
  } else {
    return KeywordTokenizer::Error{};
  }
}
KeywordTokenizer::Result get_result(auto old_state, auto new_state) {
  return KeywordTokenizer::Pending{};
}

//////////////////////////////////////////

KeywordTokenizer::Result KeywordTokenizer::step(uint8_t raw_input) {
  Input input = KeywordInput::from(raw_input);

  auto next_state = std::visit(
      [](auto real_current_state, auto real_input) {
        return transition(real_current_state, real_input);
      },
      this->state, input);
  
  auto res = std::visit(
    [next_state](auto real_old_state, auto real_new_state) {
      return get_result(real_old_state, real_new_state);
    }, this->state, next_state);

  this->state = next_state;
  return res;
}
