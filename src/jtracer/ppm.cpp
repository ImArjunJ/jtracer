#include "ppm.hpp"
#include "utils.hpp"
#include <print>
#include <stdexcept>
#include <string>
namespace jt::ppm {
file &file::parse(ppm_queue &input) {
  std::string_view header = input.front();
  input.pop();

  if (header != "P3" && header != "P6")
    throw std::runtime_error("[jt::ppm] unsupported .ppm file");

  std::string size_unparsed = input.front();
  std::vector<std::string> size_str = utils::split(size_unparsed);

  for (const auto &a : size_str) {
    if (!utils::is_number(a) || size_str.size() != 2)
      throw std::runtime_error("[jt::ppm] invalid size parameters");
  }

  this->size.x = std::stoi(size_str[0]);
  this->size.y = std::stoi(size_str[1]);

  while (!input.empty()) {
    std::string_view line = input.front();
    std::println("read {}", input.front());
    input.pop();
  }

  return *this;
}
} // namespace jt::ppm