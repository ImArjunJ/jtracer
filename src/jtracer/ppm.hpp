#include "math.hpp"
#include <fstream>
#include <queue>
#include <string>
#include <vector>
namespace jt::ppm {
typedef std::queue<std::string> ppm_queue;
class file {
public:
  file() = default;
  file(ppm_queue &input) { this->parse(input); }
  file(std::ifstream &input_file) {
    jt::ppm::ppm_queue input;
    for (std::string line; std::getline(input_file, line);)
      if (!line.starts_with('#'))
        input.push(line);
    this->parse(input);
  }
  math::vec2i get_size() { return size; }
  std::vector<math::col3> &get_data() { return data; }
  file &parse(ppm_queue &input);

private:
private:
  math::vec2i size;
  std::vector<math::col3> data;
};
} // namespace jt::ppm