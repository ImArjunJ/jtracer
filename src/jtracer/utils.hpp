#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
namespace jt::utils {
inline std::vector<std::string> split(std::string input, char delimeter = ' ',
                                      std::vector<std::string> acc = {}) {
  if (input.find(delimeter) == std::string::npos) {
    acc.push_back(input);
    return acc;
  }
  acc.push_back(input.substr(0, input.find(delimeter)));
  input.erase(0, input.find(delimeter) + 1);
  return split(input, delimeter, acc);
}
inline bool is_number(const std::string &s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](std::uint8_t c) {
                         return !std::isdigit(c);
                       }) == s.end();
}
} // namespace jt::utils