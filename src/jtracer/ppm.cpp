#include "ppm.hpp"
#include <algorithm>
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
  this->version = (header[1] == '3') ? file_version::v3 : file_version::v6;
  input_string.erase(0, 2);
  switch (this->version) {
  case file_version::v3:
    this->parse_p3(input_string);
    break;
  default:
    throw std::runtime_error(
        "[jt::ppm] version 6 and other ppms are currently unsupported");
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
      if (this->max_val == 0) {
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
bool file::write(file_version version, std::ofstream &output_file) {
  switch (version) {
  case file_version::v3: {
    output_file << "P3\n"
                << this->size.x << " " << this->size.y << "\n"
                << this->max_val << "\n";
    for (std::uint32_t i = 0; i < this->size.x; i++) {
      for (std::uint32_t j = 0; j < this->size.y; j++) {
        output_file << this->data[i][j] << "\n";
      }
    }
    break;
  }
  default:
    return false;
  }
  return true;
}
void file::push_data(std::vector<math::col3> row) {
  if (!this->data.empty()) {
    if (row.size() != this->data[0].size())
      throw std::runtime_error(
          "[jt::ppm::file] mismatched row size push attempted");
  }
  this->data.push_back(row);
  this->size.x = row.size();
  this->size.y = this->data.size();
  jt::math::col3 &max_elem = *std::max_element(row.begin(), row.end());
  if (this->max_val < max_elem.max())
    this->max_val = max_elem.max();
}
} // namespace jt::ppm