#pragma once

#include "../tokenizer.hpp"

#include <cstdint>
#include <variant>

class NumericTokenizer : public Tokenizer {
public:
    struct Initial {};
    struct Whole { uint64_t w = 0; };
    struct Decimal { uint64_t w = 0; uint64_t d = 0; uint8_t count = 0; };
    struct Invalid {};

    using State = std::variant<Initial, Whole, Decimal, Invalid>; 

    State state = Initial{};

    Result step(uint8_t raw_input) override;       
};
