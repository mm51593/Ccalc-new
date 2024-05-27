#include <istream>
#include <optional>
#include <string>

class Reader {
public:
  struct ReaderRet {
    std::optional<uint8_t> val;
    std::optional<uint8_t> operator*();
  };

  Reader(std::istream &file);
  std::optional<uint8_t> operator*();
  ReaderRet operator++();
  ReaderRet operator++(int);

private:
  struct LineLoader {
    LineLoader(Reader &rd);
    std::optional<std::string> &operator*();
    LineLoader operator++();

  private:
    Reader &rd;

    void get_next_line();
  };

  std::istream &file;
  LineLoader ll {*this};
  std::optional<std::string> line = std::nullopt;
  std::string::iterator it;
};
