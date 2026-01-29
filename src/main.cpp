// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <imgui.h>

#include <jtracer/math.hpp>
#include <jtracer/objects/sphere.hpp>
#include <jtracer/ppm.hpp>
#include <jtracer/window.hpp>

int main(int argc, char** argv) {
  if (!jt::window::create())
    throw std::runtime_error("[jt::window] failed to create window");

  std::vector<jt::objects::sphere> objects = {};
  objects.push_back(jt::objects::sphere({-30, 0, 100}, 30));
  objects.push_back(jt::objects::sphere({30, 50, 100}, 30));
  std::ofstream out_file("test.ppm");
  jt::ppm::file image;
  image.reserve(500, 500);
  for (float i = 0; i < image.get_size().x; i++) {
    for (float j = 0; j < image.get_size().y; j++) {
      const std::float_t aspect_ratio =
          static_cast<std::float_t>(image.get_size().x) / image.get_size().y;
      const jt::math::vec3f origin = {0, 0, 0};
      const jt::math::vec2f cartesian = {i - image.get_size().y / 2.f,
                                         j - image.get_size().x / 2.f};
      const jt::math::vec3f direction = {cartesian.x, cartesian.y, 100};
      jt::math::ray ray(origin, direction);

      for (auto& thing : objects) {
        jt::objects::hit_data data = thing.hit(ray);
        if (!data.hit) continue;
        // messy but it works for now, we should probably map the normal coords
        // to rgb properly
        jt::math::vec3f color_vec = {std::abs(data.normal.x),
                                     std::abs(data.normal.y),
                                     std::abs(data.normal.z)};

        image.set_data(j, i,
                       {(std::uint32_t)(color_vec.x * 255),
                        (std::uint32_t)(color_vec.y * 255),
                        (std::uint32_t)(color_vec.z * 255)});
      }
    }
  }
  image.write(jt::ppm::file_version::v3, out_file);
  out_file.close();
  jt::window::tick(image);
}