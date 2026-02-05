#pragma once
#include <cstdint>
#include <fstream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

#include "math.hpp"
namespace jt::ppm {
typedef std::queue<std::string> ppm_queue;
enum class file_version : std::uint8_t { v3 = 0, v6, none };
class file {
 public:
  file() = default;
  file(std::ifstream& input_file) { this->parse(input_file); }
  math::vec2i get_size() const { return size; }
  std::uint32_t get_max_val() const { return max_val; }
  std::vector<std::vector<math::col3>>& get_data() { return data; }
  file& parse(std::ifstream& input_file);
  bool write(file_version version, std::ofstream& output_file);
  void push_data(std::vector<math::col3> row);
  void reserve(std::uint32_t width, std::uint32_t height,
               math::col3 val = math::col3{0, 0, 0});
  void set_data(std::uint32_t x, std::uint32_t y, math::col3 value);

 private:
  void parse_p3(const std::string& input_string);
  void parse_p6(const std::string& input_string);

 private:
  math::vec2i size = {std::numeric_limits<int>::max(),
                      std::numeric_limits<int>::max()};
  std::uint32_t max_val = 255;
  file_version version = file_version::none;
  std::vector<std::vector<math::col3>> data = {};
};
}  // namespace jt::ppm