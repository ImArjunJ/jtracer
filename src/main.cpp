#include "jtracer/ppm.hpp"
#include <fstream>

int main(int argc, char **argv) {
  if (argc <= 1) {
    std::println("[jt] usage ./jtracer <path_to_ppm>");
    return -1;
  }

  std::ifstream ppm_file(argv[1]);
  if (!ppm_file.is_open()) {
    std::println("[jt] failed to open {}", argv[1]);
    return -1;
  }
  std::stringstream f;
  f << ppm_file.rdbuf();
  jt::ppm::file(f.str());
}