#include <istream>
#include <optional>
#include <string>

class Reader {
public:
  struct Iterator {
  public:
    Iterator(std::istream &file);
    std::optional<std::string> operator*();
    Iterator operator++();
    Iterator operator++(int);

  private:
    void get_next();
    std::string line;
    std::istream &file;
  };

  Reader(std::istream &file);
  Iterator begin();

private:
  std::istream &file;
};
