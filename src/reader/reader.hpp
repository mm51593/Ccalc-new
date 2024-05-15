#include <istream>
#include <optional>
#include <string>

class Reader {
public:
  Reader(std::istream &file);
  std::optional<std::string> next();

private:
  std::istream &file;
};
