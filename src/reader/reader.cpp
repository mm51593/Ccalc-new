#include "reader.hpp"
#include <iostream>
#include <istream>
#include <optional>
#include <string>

//////////////////////////////////////////

Reader::Iterator::Iterator(std::istream &file) : file(file) {}

std::optional<std::string> Reader::Iterator::operator*() {
  if (!this->file) {
    return std::nullopt;
  }
  return this->line;
}

Reader::Iterator Reader::Iterator::operator++() {
  this->get_next();
  return *this;
}
Reader::Iterator Reader::Iterator::operator++(int) {
  Reader::Iterator tmp = *this;
  ++(*this);
  return tmp;
}

void Reader::Iterator::get_next() { std::getline(this->file, this->line); }

//////////////////////////////////////////

Reader::Reader(std::istream &file) : file(file) {}

Reader::Iterator Reader::begin() { return Iterator(this->file); }
