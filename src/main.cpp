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

  ppm_file_raw.close();
}