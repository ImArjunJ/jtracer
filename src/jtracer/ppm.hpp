#include "math.hpp"
#include <print>
#include <sstream>
#include <string>
#include <vector>
namespace jt::ppm {
class file {
public:
  file() = default;
  file(const std::string &input) { this->parse(std::stringstream(input)); }
  file &parse(std::stringstream input) {
    for (std::string l; std::getline(input, l);) {
      std::println("{}", l);
    }
    return *this;
  }

private:
  math::vec2i size;
  std::vector<math::col3> data;
};
} // namespace jt::ppm