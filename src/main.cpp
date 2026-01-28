// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <imgui.h>

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "jtracer/math.hpp"
#include "jtracer/ppm.hpp"
#include "jtracer/window.hpp"

jt::math::col3 hit_sphere(jt::math::ray& ray, jt::math::vec3f center,
                          const float radius) {
  // need to find point where ray hits sphere
  // ray hits sphere for all points P s.t. |C-P| = R
  // let ray = A + tB
  // Def. L = C-A => (B.B)t^2 - 2(L.B)t + (L.L) - r^2 = 0
  const jt::math::vec3f L = center - ray.get_origin();
  const jt::math::vec3f B = ray.get_direction();
  const std::float_t delta = std::powf(2 * L.dot(B), 2) -
                             4 * (B.dot(B) * (L.dot(L) - radius * radius));
  if (delta >= 0) {
    //-b += sqrt(delta) / 2a
    jt::math::vec2f solutions = {
        ((2 * L.dot(B)) + std::sqrtf(delta)) / (2 * B.dot(B)),
        ((2 * L.dot(B)) - std::sqrtf(delta)) / (2 * B.dot(B))};
    jt::math::vec3f solution_1 = ray.at(solutions.x);
    jt::math::vec3f solution_2 = ray.at(solutions.y);
    std::cout << solution_1 << std::endl;
    return jt::math::col3{(std::uint32_t)std::abs(solution_1.x),
                          (std::uint32_t)std::abs(solution_1.y),
                          (std::uint32_t)std::abs(solution_1.z)};
  }
  return jt::math::col3{0, 0, 0};
}

int main(int argc, char** argv) {
  if (!jt::window::create())
    throw std::runtime_error("[jt::window] failed to create window");

  std::ofstream out_file("test.ppm");
  jt::ppm::file image;
  jt::math::ray ray({0, 0, 0}, {1, 0, 1});
  image.reserve(500, 500);
  for (float i = 0; i < image.get_size().y; i++) {
    for (float j = 0; j < image.get_size().x; j++) {
      const jt::math::vec3f origin = {10, 10, 30};
      const jt::math::vec2f cartesian = {j - image.get_size().x / 2.f,
                                         i - image.get_size().y / 2.f};
      const jt::math::vec3f direction = {cartesian.x, cartesian.y, 100};
      jt::math::ray ray(origin, direction);
      image.set_data(j, i, hit_sphere(ray, {0, 0, 100}, 50.f));
    }
  }
  image.write(jt::ppm::file_version::v3, out_file);
  out_file.close();
  jt::window::tick(image);
}