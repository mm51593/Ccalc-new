#pragma once

#include "../tokenizer.hpp"

#include <cstdint>
#include <string>
#include <variant>

class IdentifierTokenizer : public Tokenizer {
public:
    struct Initial {};
    struct Other { std::string name; };
    struct Invalid {};

    using State = std::variant<Initial, Other, Invalid>;

    State state = Initial{};

    Result step(uint8_t raw_input) override;
};
