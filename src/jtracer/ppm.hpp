#include "math.hpp"
#include <cstdint>
#include <fstream>
#include <limits>
#include <queue>
#include <string>
#include <vector>
namespace jt::ppm {
typedef std::queue<std::string> ppm_queue;
class file {
public:
  file() = default;
  file(std::ifstream &input_file) { this->parse(input_file); }
  math::vec2i get_size() { return size; }
  std::vector<std::vector<math::col3>> &get_data() { return data; }
  file &parse(std::ifstream &input_file);

private:
  void parse_p3(const std::string &input_string);

private:
  math::vec2i size = {std::numeric_limits<int>::max(),
                      std::numeric_limits<int>::max()};
  std::uint32_t max_val = std::numeric_limits<int>::max();
  std::vector<std::vector<math::col3>> data;
};
} // namespace jt::ppm