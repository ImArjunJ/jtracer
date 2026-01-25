#include <print>

#include "jtracer/ppm.hpp"

int main(int argc, char** argv) {
  jt::ppm::file ppm_file;
  std::ofstream file("test.ppm");
  const bool written = ppm_file.write(jt::ppm::file_version::v3, file);
  if (written) {
    std::println("written to test.ppm");
  } else {
    std::println("jt::ppm::file::write failed");
  }
  file.close();
}