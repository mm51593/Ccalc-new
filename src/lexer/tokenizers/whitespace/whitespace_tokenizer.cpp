#include "whitespace_tokenizer.hpp"

#include <cctype>
#include <cstdint>
#include <variant>

using Initial = WhitespaceTokenizer::Initial;
using Valid = WhitespaceTokenizer::Valid;
using Invalid = WhitespaceTokenizer::Invalid;

//////////////////////////////////////////

struct Whitespace {};

using Input = std::variant<Whitespace, std::monostate>;

namespace WhitespaceInput {
Input from(uint8_t raw_input) {
  if (isspace(raw_input)) {
    return Whitespace{};
  } else {
    return std::monostate();
  }
}
} // namespace WhitespaceInput

//////////////////////////////////////////

WhitespaceTokenizer::State transition(Initial state, Whitespace input) {
  return Valid{};
}
WhitespaceTokenizer::State transition(Valid state, Whitespace input) {
  return Valid{};
}
WhitespaceTokenizer::State transition(auto state, auto input) {
  return Invalid{};
}

//////////////////////////////////////////

WhitespaceTokenizer::Result get_result(Initial old_state, Invalid new_state) {
  return WhitespaceTokenizer::Error{};
}
WhitespaceTokenizer::Result get_result(Invalid old_state, Invalid new_state) {
  return WhitespaceTokenizer::Error{};
}
WhitespaceTokenizer::Result get_result(Valid old_state, Invalid new_state) {
  return WhitespaceTokenizer::Done{WhitespaceToken{}};
}
WhitespaceTokenizer::Result get_result(auto old_state, auto new_state) {
  return WhitespaceTokenizer::Pending{};
}

//////////////////////////////////////////

WhitespaceTokenizer::Result WhitespaceTokenizer::step(uint8_t raw_input) {
  Input input = WhitespaceInput::from(raw_input);

  auto next_state = std::visit([](auto real_current_state, auto real_input) {
    return transition(real_current_state, real_input);
  }, this->state, input);

  auto res = std::visit(
    [next_state](auto real_old_state, auto real_new_state) {
      return get_result(real_old_state, real_new_state);
    }, this->state, next_state);
  
  this->state = next_state;
  return res;
}
