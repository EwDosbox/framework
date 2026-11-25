#include "application.hpp"

#include "SceneObject.hpp"

#include <glm/fwd.hpp>
#include <memory>
#include <vector>

using namespace std;

Application::Application(int initial_width, int initial_height,
                         std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {
  this->width = initial_width;
  this->height = initial_height;

  images_path = lecture_folder_path / "images";
  objects_path = lecture_folder_path / "objects";
  shaders_path = lecture_folder_path / "shaders";

  forest = std::make_unique<World>(vec3(20), lecture_folder_path);
  forest->Initialize();

  compile_shaders();
}

Application::~Application() { delete_shaders(); }

#pragma region Methods

void Application::delete_shaders() { SceneObject::DeleteShaders(); }

void Application::compile_shaders() {
  delete_shaders();
  SceneObject::InitShaders(shaders_path);
}

void Application::update(float delta) { forest->advance_time(delta); }

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  vec3 light_position = forest->GetLightPosition();

  for (auto &object : forest->GetObjects()) {
    object->draw(light_position);
  }

  for (auto &object : forest->GetInstantiatedObjects()) {
    object->draw_instantiated(light_position);
  }
}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

#pragma endregion
#pragma region Input Events

void Application::on_resize(int width, int height) {
  // Calls the default implementation to set the class variables.
  PV112Application::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) {
  camera.on_mouse_move(x, y);
}
void Application::on_mouse_button(int button, int action, int mods) {
  camera.on_mouse_button(button, action, mods);
}
void Application::on_key_pressed(int key, int scancode, int action, int mods) {
  // Calls default implementation that invokes compile_shaders when 'R key is
  // hit.
  PV112Application::on_key_pressed(key, scancode, action, mods);
}

#pragma endregion