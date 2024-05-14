#include "lexer.hpp"
#include "tokenizers/tokenizer.hpp"

#include <cstdint>
#include <tuple>
#include <variant>
#include <vector>

const Lexer::Tokenizers Lexer::tokenizers_template = {
    WhitespaceTokenizer{},
    NumericTokenizer{},
    OperatorTokenizer{},
    IdentifierTokenizer{},
    KeywordTokenizer{},
};

//////////////////////////////////////////

template <class T>
concept can_tokenize = std::is_base_of<Tokenizer, T>::value;

template <can_tokenize T>
Tokenizer::Result feed_char(T &tokenizer, uint8_t raw_input) {
  return tokenizer.step(raw_input);
}

//////////////////////////////////////////

Token Lexer::get_next(std::string::iterator &it) {
  Tokenizers tokenizers = this->tokenizers_template;

  std::vector<Token> products;
  products.reserve(tknzr_size);

  std::array<Result, tknzr_size> results;
  uint8_t results_index;

  bool shouldLoop;
  do {
    shouldLoop = false;
    results_index = 0;
    std::apply(
        [it, &results, &results_index](auto &...args) {
          ((results[results_index++] = feed_char(args, *it)), ...);
        },
        tokenizers);

    for (auto res : results) {
      if (Tokenizer::Pending *real_res =
              std::get_if<Tokenizer::Pending>(&res)) {
        shouldLoop = true;
      } else if (Tokenizer::Done *real_res =
                     std::get_if<Tokenizer::Done>(&res)) {
        products.push_back(real_res->token);
      }
    }

    if (shouldLoop) {
      it++;
    }
  } while (shouldLoop);

  return products.back();
}
