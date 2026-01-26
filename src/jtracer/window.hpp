#pragma once
#include "ppm.hpp"
namespace jt::window {
bool create();
void tick(ppm::file& image);
}  // namespace jt::window