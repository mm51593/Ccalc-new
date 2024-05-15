#include "reader.hpp"
#include <iostream>
#include <istream>
#include <optional>
#include <string>

Reader::Reader(std::istream &file) : file(file) {}

std::optional<std::string> Reader::next() {
  if (!this->file) {
    return std::nullopt;
  }
  std::string line;
  std::getline(this->file, line);
  return line;
}
