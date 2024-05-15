#include "../src/reader/reader.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <optional>
#include <string>

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
  std::optional<std::string> line;

  do {
    line = rd.next();
    if (line.has_value()) {
      std::cout << line.value() << '\n';
    }
  } while (line.has_value());
}
