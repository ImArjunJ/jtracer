#include <cstdint>
namespace jt::math {
template <typename T> struct vec2_t {
  T x, y;
};
typedef vec2_t<float> vec2f;
typedef vec2_t<int> vec2i;
struct col3 {
  std::uint32_t r, g, b;
};
} // namespace jt::math