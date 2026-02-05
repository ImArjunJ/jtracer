#include "window.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <jtracer/math.hpp>
#include <jtracer/objects/sphere.hpp>
#include <jtracer/ppm.hpp>
#include <pulse.hpp>
namespace jt::window {
struct jt_window_t {
  GLFWwindow* window = nullptr;
  std::int32_t width, height;
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
void render() {
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui_ImplGlfw_InitForOpenGL(jt_window.window, true);
  ImGui_ImplOpenGL3_Init();

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  std::vector<jt::objects::sphere> objects = {};
  objects.push_back(jt::objects::sphere({-30, 0, 100}, 30));
  objects.push_back(jt::objects::sphere({30, 50, 100}, 30));

  std::vector<unsigned char> raw_image;

  std::float_t fov = 80;
  jt::math::vec3f origin = {0, 0, 0};

  jt::math::vec3f light_dir = {-100, 10, 0};
  std::float_t color[3] = {1.f, 230 / 255.f, 0.f};
  while (!glfwWindowShouldClose(jt_window.window)) {
    std::float_t tan_half_fov = std::tanf(fov * (std::numbers::pi / 360.f));
    const jt::math::vec3f norm_light_dir = light_dir.normalize();
    glfwPollEvents();
    glfwGetWindowSize(jt_window.window, &jt_window.width, &jt_window.height);
    int img_w = jt_window.width;
    int img_h = jt_window.height;

    if (img_w > 0 && img_h > 0) {
      if (raw_image.size() != img_w * img_h * 3) {
        raw_image.resize(img_w * img_h * 3);
      }

      glBindTexture(GL_TEXTURE_2D, textureID);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, raw_image.data());
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (img_w > 0 && img_h > 0) {
      ImGui::GetBackgroundDrawList()->AddImage(
          (void*)(intptr_t)textureID, ImVec2(0, 0),
          ImVec2((float)jt_window.width, (float)jt_window.height));
    }

    ImGui::DockSpaceOverViewport(0, NULL,
                                 ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::Begin("Ray Tracing Configuration");

    if (img_w > 0 && img_h > 0) {
      ImGui::Text("Image: %dx%d", img_w, img_h);
    } else {
      ImGui::Text("Waiting for image data...");
    }

    if (ImGui::Button("Render")) {
      std::fill(raw_image.begin(), raw_image.end(), 0);
      for (int i = 0; i < img_h; i++) {
        for (int j = 0; j < img_w; j++) {
          const jt::math::vec2f cartesian = {j - img_w / 2.f, i - img_h / 2.f};
          jt::math::vec3f direction = {cartesian.x, cartesian.y,
                                       (img_w / 2.f) / tan_half_fov};

          jt::math::ray ray(origin, direction);

          for (auto& thing : objects) {
            jt::objects::hit_data data = thing.hit(ray);
            if (!data.hit) continue;

            std::float_t radiance = (data.normal.dot(norm_light_dir));
            radiance = std::max(radiance, 0.f);

            const std::uint32_t location = (j + (i * img_w)) * 3;
            raw_image[location + 0] = (std::uint8_t)(radiance * color[0] * 255);
            raw_image[location + 1] = (std::uint8_t)(radiance * color[1] * 255);
            raw_image[location + 2] = (std::uint8_t)(radiance * color[2] * 255);
          }
        }
      }
    }
    ImGui::DragFloat("Camera FOV", &fov);
    ImGui::ColorPicker3("Sphere Picker", color);

    ImGui::End();
    ImGui::Render();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(jt_window.window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}
}  // namespace jt::window
