// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <imgui.h>

#include <jtracer/window.hpp>
#include <pulse.hpp>

int main(int argc, char** argv) {
  PULSE_INIT("jtracer.json");
  if (!jt::window::create())
    throw std::runtime_error("[jt::window] failed to create window");

  jt::window::render();
  PULSE_SHUTDOWN();
}
