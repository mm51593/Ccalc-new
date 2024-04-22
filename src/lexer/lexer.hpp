#include "token.hpp"
#include "tokenizers/whitespace/whitespace_tokenizer.hpp"
#include "tokenizers/identifier/identifier_tokenizer.hpp"
#include "tokenizers/keyword/keyword_tokenizer.hpp"
#include "tokenizers/numeric/numeric_tokenizer.hpp"
#include "tokenizers/operator/operator_tokenizer.hpp"
#include "tokenizers/tokenizer.hpp"

#include <cstdint>
#include <tuple>

class Lexer {
public:
  using Tokenizers = std::tuple<WhitespaceTokenizer, NumericTokenizer, OperatorTokenizer,
                                IdentifierTokenizer, KeywordTokenizer>;
  constexpr static uint8_t tknzr_size = std::tuple_size<Tokenizers>();
  using Result = Tokenizer::Result;

  Token get_next(std::string::iterator &it);

private:
  const static Tokenizers tokenizers_template;
};
