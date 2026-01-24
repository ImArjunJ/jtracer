#include "ppm.hpp"
#include <limits>
#include <sstream>
#include <stdexcept>
namespace jt::ppm {
file &file::parse(std::ifstream &input) {
  std::stringstream ss;
  ss << input.rdbuf();
  std::string input_string = ss.str();
  std::string header = input_string.substr(0, 2);
  if (header != "P3" && header != "P6")
    throw std::runtime_error("[jt::ppm] invalid header");
  input_string.erase(0, 2);

  switch (header[1]) {
  case '3':
    this->parse_p3(input_string);
    break;
  case '6':
    throw std::runtime_error(
        "[jt::ppm] version 6 ppms are currently unsupported");
    break;
  }

  return *this;
}
void file::parse_p3(const std::string &input_string) {
  bool in_comment = false;
  bool parsing_num = false;
  std::string current_num = "";
  std::vector<std::uint32_t> data = {};
  for (int i = 0; i < input_string.size(); i++) {
    char ch = input_string[i];
    if (ch == '\n')
      in_comment = false;
    if (in_comment)
      continue;
    if (ch == '#')
      in_comment = true;
    if (!std::isdigit(ch) && ch != ' ' && ch != '\n')
      continue;

    if (std::isdigit(ch)) {
      current_num += ch;
    }
    if ((ch == ' ' || ch == '\n') && current_num != "") {
      if (this->size.x == std::numeric_limits<int>::max()) {
        this->size.x = std::stoi(current_num);
        current_num = "";
        continue;
      }
      if (this->size.y == std::numeric_limits<int>::max()) {
        this->size.y = std::stoi(current_num);
        current_num = "";
        continue;
      }
      if (this->max_val == std::numeric_limits<int>::max()) {
        this->max_val = std::stoi(current_num);
        current_num = "";
        continue;
      }
      data.push_back(std::stoi(current_num));
      current_num = "";
    }
  }
  if (data.size() % 3 != 0)
    data.push_back(std::stoi(current_num));

  std::vector<math::col3> row = {};
  for (int i = 0; i < data.size(); i += 3) {
    math::col3 curr_col = {data[i], data[i + 1], data[i + 2]};
    row.push_back(curr_col);
    if (row.size() == this->size.x) {
      this->data.push_back(row);
      row.clear();
    }
  }
}
} // namespace jt::ppm