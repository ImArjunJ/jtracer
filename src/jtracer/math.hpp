#pragma once
#include <cmath>
#include <cstdint>
#include <format>
#include <ostream>
namespace jt::math {
template <typename T>
struct vec2_t {
  T x, y;
};
typedef vec2_t<float> vec2f;
typedef vec2_t<int> vec2i;

template <typename T>
struct vec3_t {
  T x, y, z;
  friend vec3_t<T> operator+(const vec3_t<T> a, const vec3_t<T> b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
  }
  friend vec3_t<T> operator-(const vec3_t<T> a, const vec3_t<T> b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
  }
  friend vec3_t<T> operator*(const vec3_t<T> a, const T b) {
    return {a.x * b, a.y * b, a.z * b};
  }
  friend vec3_t<T> operator/(const vec3_t<T> a, const T b) {
    return {a.x / b, a.y / b, a.z / b};
  }
  friend std::ostream& operator<<(std::ostream& out, const vec3_t<T> vec) {
    out << std::format("{},{},{}", vec.x, vec.y, vec.z);
    return out;
  }
  T magnitude() const {
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
  }
  T dot(vec3_t<T> other) const {
    return this->x * other.x + this->y * other.y + this->z * other.z;
  }
  vec3_t<T> normalize() const { return *this / magnitude(); }
};

typedef vec3_t<float> vec3f;
typedef vec3_t<std::int32_t> vec3i;
typedef vec3_t<std::uint32_t> vec3ui;
typedef vec3_t<std::uint8_t> vec3u8;

// a ray is in the form A + tB s.t. A is the origin vec, B is the direction
// vec
class ray {
 public:
  ray(vec3f origin, vec3f direction)
      : m_origin(origin), m_direction(direction) {}

  vec3f at(std::float_t t) { return this->m_origin + (this->m_direction * t); }
  vec3f get_direction() { return m_direction; }
  vec3f get_origin() { return m_origin; }

 private:
  vec3f m_origin, m_direction;
};

struct col3 : vec3u8 {
  std::uint8_t r() const { return this->x; }
  std::uint8_t g() const { return this->y; }
  std::uint8_t b() const { return this->z; }
  std::uint8_t max() { return std::max(std::max(r(), g()), b()); }
  friend bool operator<(col3 lhs, col3 rhs) {
    return lhs.r() < rhs.r() || lhs.g() < rhs.g() || lhs.b() < rhs.b();
  }
  friend bool operator>(col3 lhs, col3 rhs) {
    return lhs.r() > rhs.r() || lhs.g() > rhs.g() || lhs.b() > rhs.b();
  }
  friend std::ostream& operator<<(std::ostream& out, const col3& vec) {
    out << std::format("{} {} {}", vec.x, vec.y, vec.z);
    return out;
  }
};
}  // namespace jt::math

template <>
struct std::formatter<jt::math::vec3f> {
  constexpr auto parse(std::format_parse_context& context) {  // (4)
    return context.begin();
  }
  auto format(const jt::math::vec3f& vec,
              std::format_context& context) const {  // (5)
    return std::format_to(context.out(), "{},{},{}", vec.x, vec.y, vec.z);
  }
};