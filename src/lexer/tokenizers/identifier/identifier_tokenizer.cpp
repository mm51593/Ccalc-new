#include "identifier_tokenizer.hpp"

#include <cctype>
#include <cstdint>
#include <variant>
#include <optional>

using Initial = IdentifierTokenizer::Initial;
using Other = IdentifierTokenizer::Other;
using Invalid = IdentifierTokenizer::Invalid;

//////////////////////////////////////////

struct Letter {
  uint8_t c;
};

struct NonLetter {
  uint8_t c;
};

using Input = std::variant<Letter, NonLetter, std::monostate>;

namespace IdentifierInput {
Input from(uint8_t raw_input) {
  if (isalpha(raw_input)) {
    return Letter{.c = raw_input};
  } else if (isdigit(raw_input) || raw_input == '_') {
    return NonLetter{.c = raw_input};
  } else {
    return std::monostate();
  }
}
} // namespace IdentifierInput

//////////////////////////////////////////

IdentifierTokenizer::State transition(Initial &state, Letter &input) {
  Other ret;
  ret.name.push_back(input.c);
  return ret;
}
IdentifierTokenizer::State transition(Other &state, Letter &input) {
  state.name.push_back(input.c);
  return state;
}
IdentifierTokenizer::State transition(Other &state, NonLetter &input) {
  state.name.push_back(input.c);
  return state;
}
IdentifierTokenizer::State transition(auto &state, auto &input) {
  return Invalid{};
}

//////////////////////////////////////////

std::optional<Token> produce_result(Other final_state) {
  return IdentifierToken{final_state.name};
}

//////////////////////////////////////////

IdentifierTokenizer::Result get_result(Initial old_state, Invalid new_state) {
  return IdentifierTokenizer::Error{};
}
IdentifierTokenizer::Result get_result(Invalid old_state, Invalid new_state) {
  return IdentifierTokenizer::Error{};
}
IdentifierTokenizer::Result get_result(Other old_state, Invalid new_state) {
  return IdentifierTokenizer::Done{produce_result(old_state).value()};
}
IdentifierTokenizer::Result get_result(auto old_state, auto new_state) {
  return IdentifierTokenizer::Pending{};
}

//////////////////////////////////////////

IdentifierTokenizer::Result IdentifierTokenizer::step(uint8_t raw_input) {
  Input input = IdentifierInput::from(raw_input); 

  auto next_state = std::visit([](auto real_current_state, auto real_input) {
    return transition(real_current_state, real_input);
  }, this->state, input);

  auto res = std::visit([](auto real_old_state, auto real_new_state) {
    return get_result(real_old_state, real_new_state);
  }, this->state, next_state);

  this->state = next_state;
  return res;
}
