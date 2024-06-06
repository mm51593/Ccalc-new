cmake_minimum_required(VERSION 3.20)
project(Ccalc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)

add_executable(TestReader test/test_reader.cpp src/reader/reader.cpp)
add_executable(TestLexer test/test_lexer.cpp src/reader/reader.cpp src/lexer/lexer.cpp src/lexer/tokenizers/operator/operator_tokenizer.cpp src/lexer/tokenizers/keyword/keyword_tokenizer.cpp src/lexer/tokenizers/numeric/numeric_tokenizer.cpp src/lexer/tokenizers/whitespace/whitespace_tokenizer.cpp src/lexer/tokenizers/identifier/identifier_tokenizer.cpp)
