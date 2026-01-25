#include <cstdint>
#include <ostream>
namespace jt::math {
template <typename T> struct vec2_t {
  T x, y;
};
typedef vec2_t<float> vec2f;
typedef vec2_t<int> vec2i;
struct col3 {
  std::uint32_t r, g, b;
  std::uint32_t max() { return std::max(std::max(r, g), b); }
  friend std::ostream &operator<<(std::ostream &ss, col3 &col) {
    ss << col.r << ' ' << col.g << ' ' << col.b;
    return ss;
  }
  friend bool operator<(col3 &lhs, col3 &rhs) {
    return lhs.r < rhs.r || lhs.g < rhs.g || lhs.b < rhs.b;
  }
  friend bool operator>(col3 &lhs, col3 &rhs) {
    return lhs.r > rhs.r || lhs.r > rhs.r || lhs.b > rhs.b;
  }
};

} // namespace jt::math