#pragma once 

#include "../token.hpp"
#include <cstdint>

class Tokenizer {
public:
    struct Pending {};
    struct Done { Token token; };
    struct Error {};

    using Result = std::variant<Pending, Done, Error>;

    virtual Result step(uint8_t input) = 0;
};
