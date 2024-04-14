#pragma once

#include "../tokenizer.hpp"

#include <cstdint>
#include <variant>

class NumericTokenizer : public Tokenizer {
public:
    struct Initial {};
    struct Whole { uint64_t x = 0; };
    struct Decimal { uint64_t x = 0; };
    struct Invalid {};

    using State = std::variant<Initial, Whole, Decimal, Invalid>; 

    State state = Initial{};

    Result step(uint8_t raw_input) override;       

    Token produce_result() override;

    void update(auto state);

private:
    uint64_t whole = 0;
    uint64_t decimal = 0;
};
