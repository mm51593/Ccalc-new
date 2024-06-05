#include "../src/lexer/lexer.hpp"
#include "../src/reader/reader.hpp"
#include "src/lexer/token.hpp"
#include <format>
#include <iostream>
#include <optional>
#include <variant>

std::string to_string(NumericToken tkn) {
  return std::format("Numeric: {}", tkn.value);
}
std::string to_string(OperatorToken tkn) {
  std::string type;
  switch (tkn.type) {
  case OperatorToken::OperatorType::Add:
    type = "Add";
    break;
  case OperatorToken::OperatorType::Subtract:
    type = "Sub";
    break;
  case OperatorToken::OperatorType::Multiply:
    type = "Mul";
    break;
  case OperatorToken::OperatorType::Divide:
    type = "Div";
    break;
  case OperatorToken::OperatorType::Assign:
    type = "As";
    break;
  case OperatorToken::OperatorType::EQ:
    type = "Eq";
    break;
  case OperatorToken::OperatorType::NE:
    type = "Ne";
    break;
  case OperatorToken::OperatorType::LT:
    type = "Lt";
    break;
  case OperatorToken::OperatorType::GT:
    type = "Gt";
    break;
  case OperatorToken::OperatorType::LTE:
    type = "Lte";
    break;
  case OperatorToken::OperatorType::GTE:
    type = "Gte";
    break;
  case OperatorToken::OperatorType::LParen:
    type = "Lpar";
    break;
  case OperatorToken::OperatorType::RParen:
    type = "Rpar";
    break;
  }
  return std::format("Operator: {}", type);
}

std::string to_string(KeywordToken tkn) {
  std::string type;
  switch (tkn.type) {
  case KeywordToken::KeywordType::If:
    type = "If";
    break;
  case KeywordToken::KeywordType::Else:
    type = "Else";
    break;
  case KeywordToken::KeywordType::Then:
    type = "Then";
    break;
  case KeywordToken::KeywordType::Begin:
    type = "Begin";
    break;
  case KeywordToken::KeywordType::End:
    type = "End";
    break;
  case KeywordToken::KeywordType::While:
    type = "While";
    break;
  case KeywordToken::KeywordType::Func:
    type = "Func";
    break;
  }
  return std::format("Keyword: {}", type);
}
std::string to_string(IdentifierToken tkn) {
  return std::format("Identifier: {}", tkn.name);
}
std::string to_string(WhitespaceToken tkn) { return "Whitespace"; }

int main(void) {
  Reader rd(std::cin);
  Lexer lx(rd);

  while ((*lx).has_value()) {
    Token tkn = (*lx).value();
    std::cout << std::visit([](auto real_tkn) { return to_string(real_tkn); }, tkn) << '\n';
    ++lx;
  }
}
