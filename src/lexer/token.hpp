#pragma once 

#include <variant>

struct NumericToken { double value = 0; };

using Token = std::variant<NumericToken>;
