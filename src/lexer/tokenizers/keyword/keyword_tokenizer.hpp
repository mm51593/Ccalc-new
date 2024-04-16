#pragma once

#include "../tokenizer.hpp"

#include <array>
#include <cstdint>
#include <variant>

const uint8_t KW_MAX_LENGTH = 5;

struct KwMapping {
    std::array<uint8_t, KW_MAX_LENGTH + 1> text;
    KeywordToken::KeywordType type;
};

const KwMapping KW_MAPPINGS[] = {
    {"if", KeywordToken::KeywordType::If},
    {"else", KeywordToken::KeywordType::Else},
    {"then", KeywordToken::KeywordType::Then},
    {"begin", KeywordToken::KeywordType::Begin},
    {"end", KeywordToken::KeywordType::End},
    {"while", KeywordToken::KeywordType::While},
    {"func", KeywordToken::KeywordType::Func},
};

class KeywordTokenizer : public Tokenizer {
public:
    struct Initial {};
    struct Valid {
        std::array<uint8_t, KW_MAX_LENGTH + 1> text = {0};
        uint8_t idx = 0;
    };
    struct Invalid {};

    using State = std::variant<Initial, Valid, Invalid>;

    State state = Initial{};

    Result step(uint8_t raw_input) override;
};
