#include "../src/reader/reader.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>

int main(int argc, char *argv[]) {
  std::ifstream fd;
  std::istream &file = argc >= 2 ? [&fd, &argv]() -> std::istream & {
    fd.open(argv[1]);
    if (!fd)
      abort();
    return fd;
  }()
      : std::cin;

  Reader rd(file); 

  while (uint8_t c = rd.next().value_or(0)) {
    std::cout << c;
  }
}
