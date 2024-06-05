#pragma once

#include "token.hpp"
#include "tokenizers/tokenizers.hpp"
#include "src/reader/reader.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <tuple>

class Lexer {
public:
  using Tokenizers =
      std::tuple<WhitespaceTokenizer, NumericTokenizer, OperatorTokenizer,
                 IdentifierTokenizer, KeywordTokenizer>;
  constexpr static uint8_t tknzr_size = std::tuple_size<Tokenizers>();

  using Result = Tokenizer::Result;

  Lexer(Reader &rd);
  std::optional<Token> operator*();
  Lexer &operator++();

private:
  const static Tokenizers tokenizers_template;
  void tokenizer_loop(Tokenizers &tokeniers, std::vector<Token> &products);
  bool sort_results(std::vector<Token> &products,
                    std::array<Result, tknzr_size> &results);
  void feed_tokenizers(uint8_t c, std::array<Result, tknzr_size> &results,
                       uint8_t &results_index, Tokenizers &tokenizers);
  std::optional<Token> tokenize();

  Reader &rd;
  std::optional<Token> tkn = std::nullopt;
};
