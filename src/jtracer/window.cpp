#include "window.hpp"
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
// clang-format on
namespace jt::window {
struct jt_window_t {
  GLFWwindow* window = nullptr;
} jt_window;
bool create() {
  glfwInit();
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  jt_window.window = glfwCreateWindow(800, 800, "jtracer", NULL, NULL);
  if (!jt_window.window) {
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(jt_window.window);
  gladLoaderLoadGL();
  return jt_window.window != nullptr;
}
void tick(ppm::file& image) {
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui_ImplGlfw_InitForOpenGL(jt_window.window, true);
  ImGui_ImplOpenGL3_Init();

  while (!glfwWindowShouldClose(jt_window.window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    ImGui::Begin("Ray Tracing Configuration");
    ImGui::End();
    ImGui::Render();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(jt_window.window);
  }
  glfwTerminate();
}
}  // namespace jt::window