#include "lexer.hpp"
#include "src/lexer/token.hpp"
#include "tokenizers/tokenizer.hpp"

#include <cstdint>
#include <tuple>
#include <variant>
#include <vector>

#define INVALID_CHAR 0

const Lexer::Tokenizers Lexer::tokenizers_template = {
    WhitespaceTokenizer{}, NumericTokenizer{}, OperatorTokenizer{},
    IdentifierTokenizer{}, KeywordTokenizer{},
};

//////////////////////////////////////////

template <class T>
concept can_tokenize = std::is_base_of<Tokenizer, T>::value;

template <can_tokenize T>
Tokenizer::Result feed_char(T &tokenizer, uint8_t raw_input) {
  return tokenizer.step(raw_input);
}

//////////////////////////////////////////

Lexer::Lexer(Reader &rd) : rd(rd) {}

bool Lexer::sort_results(std::vector<Token> &products,
                         std::array<Result, tknzr_size> &results) {
  bool ret = false;
  for (auto res : results) {
    if (Tokenizer::Pending *real_res = std::get_if<Tokenizer::Pending>(&res)) {
      ret = true;
    } else if (Tokenizer::Done *real_res = std::get_if<Tokenizer::Done>(&res)) {
      products.push_back(real_res->token);
    }
  }
  return ret;
}

void Lexer::feed_tokenizers(uint8_t c, std::array<Result, tknzr_size> &results,
                            uint8_t &results_index, Tokenizers &tokenizers) {

  std::apply(
      [c, &results, &results_index](auto &...args) {
        ((results[results_index++] = feed_char(args, c)), ...);
      },
      tokenizers);
}

void Lexer::tokenizer_loop(Tokenizers &tokenizers,
                           std::vector<Token> &products) {
  std::array<Result, tknzr_size> results;
  uint8_t results_index;
  bool should_loop;

  do {
    uint8_t c = (*(this->rd)).value_or(INVALID_CHAR);
    results_index = 0;

    feed_tokenizers(c, results, results_index, tokenizers);
    should_loop = sort_results(products, results);
  } while (should_loop);
}

Token Lexer::next() {
  Tokenizers tokenizers = this->tokenizers_template;

  std::vector<Token> products;
  products.reserve(tknzr_size);

  tokenizer_loop(tokenizers, products);

  return products.back();
}
