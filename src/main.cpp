// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <imgui.h>

#include <stdexcept>

#include "jtracer/math.hpp"
#include "jtracer/ppm.hpp"
#include "jtracer/window.hpp"

int main(int argc, char** argv) {
  if (!jt::window::create())
    throw std::runtime_error("[jt::window] failed to create window");

  std::ofstream out_file("test.ppm");
  jt::ppm::file image;
  jt::math::ray ray({0, 0, 0}, {1, 0, 1});
  image.reserve(2000, 2000);
  for (int i = 0; i < image.get_size().x; i++) {
    for (int j = 0; j < image.get_size().y; j++) {
      double x0 = (i - image.get_size().x / 2.0) * 4.0 / image.get_size().x;
      double y0 = (j - image.get_size().y / 2.0) * 4.0 / image.get_size().y;
      double x = 0, y = 0;
      int iteration = 0;
      int max_iteration = 100;
      while (x * x + y * y <= 4 && iteration < max_iteration) {
        double xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        iteration++;
      }
      std::uint32_t color = (iteration * 255) / max_iteration;
      image.set_data(i, j, jt::math::col3{color, color, color});
    }
  }
  image.write(jt::ppm::file_version::v3, out_file);
  out_file.close();
  jt::window::tick(image);
}