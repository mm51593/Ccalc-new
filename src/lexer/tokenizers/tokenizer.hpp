#pragma once 

#include "../token.hpp"

#include <cstdint>
#include <optional>

class Tokenizer {
public:
    struct Pending {};
    struct Done { Token token; };
    struct Error {};

    using Result = std::variant<Pending, Done, Error>;

    virtual Result step(uint8_t input) = 0;
    virtual std::optional<Token> produce_result() = 0;
};
