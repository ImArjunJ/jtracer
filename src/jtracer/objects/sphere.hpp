#pragma once

#include "hit_object.hpp"

namespace jt::objects {
class sphere : public hit_object {
 public:
  sphere(math::vec3f center, std::float_t radius)
      : m_center(center), m_radius(radius) {}
  hit_data hit(math::ray& ray) override {
    // need to find point where ray hits sphere
    // ray hits sphere for all points P s.t. |C-P| = R
    // let ray = A + tB
    // Def. L = C-A => (B.B)t^2 - 2(L.B)t + (L.L) - r^2 = 0
    const jt::math::vec3f L = m_center - ray.get_origin();
    const jt::math::vec3f B = ray.get_direction();
    const std::float_t delta =
        std::powf(2 * L.dot(B), 2) -
        4 * (B.dot(B) * (L.dot(L) - m_radius * m_radius));
    if (delta >= 0) {
      //-b += sqrt(delta) / 2a
      const std::float_t t =
          ((2 * L.dot(B)) - std::sqrtf(delta)) / (2 * B.dot(B));
      jt::math::vec3f intersect_vec = ray.at(t);

      return {true, t, intersect_vec, (intersect_vec - m_center) / m_radius};
    }
    return {false};
  }

 private:
  math::vec3f m_center = {};
  std::float_t m_radius = 0;
};
}  // namespace jt::objects