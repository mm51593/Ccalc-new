#include "src/lexer/lexer.hpp"
#include "src/lexer/token.hpp"

#include <iostream>
#include <string>
#include <variant>

int main(void) {
	std::string line;
	std::getline(std::cin, line);

	Lexer lexer;
	auto it = line.begin();

	while (*it != '\0') {
		auto token = lexer.get_next(it);

		if (NumericToken *num = std::get_if<NumericToken>(&token)) {
			std::cout << "Numeric: " << num->value;
		} else if (IdentifierToken *id = std::get_if<IdentifierToken>(&token)) {
		    std::cout << "Identifier: " << id->name;
		} else if (KeywordToken *kw = std::get_if<KeywordToken>(&token)) {
			std::cout << "Keyword: ";
			switch (kw->type) {
			case KeywordToken::KeywordType::If:
				std::cout << "if";
				break;
			case KeywordToken::KeywordType::Else:
				std::cout << "else";
				break;
			case KeywordToken::KeywordType::Then:
				std::cout << "then";
				break;
			case KeywordToken::KeywordType::Begin:
				std::cout << "begin";
				break;
			case KeywordToken::KeywordType::End:
				std::cout << "end";
				break;
			case KeywordToken::KeywordType::While:
				std::cout << "while";
				break;
			case KeywordToken::KeywordType::Func:
				std::cout << "func";
			  break;
			}
		} else if (OperatorToken *op = std::get_if<OperatorToken>(&token)) {
			std::cout << "Operator: ";
			switch (op->type) {
			case OperatorToken::OperatorType::Add:
				std::cout << "add";
				break;
			case OperatorToken::OperatorType::Subtract:
				std::cout << "sub";
				break;
			case OperatorToken::OperatorType::Multiply:
				std::cout << "mult";
				break;
			case OperatorToken::OperatorType::Divide:
				std::cout << "div";
				break;
			case OperatorToken::OperatorType::Assign:
				std::cout << "assign";
				break;
			case OperatorToken::OperatorType::EQ:
				std::cout << "eq";
				break;
			case OperatorToken::OperatorType::NE:
				std::cout << "ne";
				break;
			case OperatorToken::OperatorType::LT:
				std::cout << "lt";
				break;
			case OperatorToken::OperatorType::GT:
				std::cout << "gt";
				break;
			case OperatorToken::OperatorType::LTE:
				std::cout << "lte";
				break;
			case OperatorToken::OperatorType::GTE:
				std::cout << "gte";
				break;
			}
		} else {
			std::cout << "Error!";
		}
		std::cout << "\n";
	}

        return 0; }
