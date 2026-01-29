#pragma once
#include <cmath>
#include <jtracer/math.hpp>
#include <limits>

namespace jt::objects {
struct hit_data {
  bool hit = false;
  std::float_t t = std::numeric_limits<std::float_t>::min();
  math::vec3f point = {};
  math::vec3f normal = {};
};

class hit_object {
 public:
  hit_object() = default;
  virtual hit_data hit(math::ray& ray) = 0;
};
}  // namespace jt::objects