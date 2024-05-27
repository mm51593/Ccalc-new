#include "reader.hpp"
#include <cstdint>
#include <istream>
#include <optional>
#include <string>

//////////////////////////////////////////

Reader::LineLoader::LineLoader(Reader &rd) : rd(rd) {}

std::optional<std::string> &Reader::LineLoader::operator*() {
  if (!this->rd.line.has_value()) {
    this->get_next_line();
  }

  return this->rd.line;
}

Reader::LineLoader Reader::LineLoader::operator++() {
  this->rd.line = std::nullopt;
  return *this;
}

void Reader::LineLoader::get_next_line() {
  if (!this->rd.file) {
    this->rd.line = std::nullopt;
  } else {
    std::string next_line;
    std::getline(this->rd.file, next_line);
    this->rd.line = next_line + '\n';
    this->rd.it = this->rd.line->begin();
  }
}

//////////////////////////////////////////

Reader::Reader(std::istream &file) : file(file) {}

std::optional<uint8_t> Reader::next() {
  while ((*this->ll).has_value() && this->it == (*this->ll).value().end()) {
    ++(this->ll);
  }

  if (!(*this->ll).has_value()) {
    return std::nullopt;
  } else {
    return *this->it++;
  }
}
