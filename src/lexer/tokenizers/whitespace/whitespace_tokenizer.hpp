#include "../tokenizer.hpp"

#include <cstdint>
#include <variant>

class WhitespaceTokenizer : public Tokenizer {
public:
    struct Initial {};
    struct Valid {};
    struct Invalid {};

    using State = std::variant<Initial, Valid, Invalid>;

    State state = Initial{};

    Result step(uint8_t raw_input) override;
};
