#include "jtracer/ppm.hpp"
#include <fstream>
#include <print>

int main(int argc, char **argv) {
  if (argc <= 1) {
    std::println("[jt] usage ./jtracer <path_to_ppm>");
    return -1;
  }

  std::ifstream ppm_file_raw(argv[1]);
  if (!ppm_file_raw.is_open()) {
    std::println("[jt] failed to open {}", argv[1]);
    return -1;
  }

  jt::ppm::file ppm_file(ppm_file_raw);

  for (auto &row : ppm_file.get_data()) {
    for (auto &thing : row) {
      std::print("({}, {}, {}) ", thing.r, thing.g, thing.b);
    }
    std::println();
  }

  jt::ppm::file test_write;
  std::println("initial test_write data: {}, {}, max_val: {}",
               test_write.get_size().x, test_write.get_size().y,
               test_write.get_max_val());
  for (std::uint32_t i = 0; i < 256; i++) {
    std::vector<jt::math::col3> row = {};
    for (std::uint32_t j = 0; j < 256; j++) {
      row.push_back(jt::math::col3{i, j, 255 - j});
    }
    test_write.push_data(row);
  }
  std::println("final test_write data: {}, {}, max_val: {}",
               test_write.get_size().x, test_write.get_size().y,
               test_write.get_max_val());
  std::ofstream test_write_file("test/test_write.ppm");
  test_write.write(jt::ppm::file_version::v3, test_write_file);
  test_write_file.close();
  ppm_file_raw.close();
}