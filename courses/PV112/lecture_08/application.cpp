// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#define STB_IMAGE_IMPLEMENTATION
#include "application.hpp"
#include "stb_image.h"


// ----------------------------------------------------------------------------
// Helper Methods.
// ----------------------------------------------------------------------------

float random_pos() {
  return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) + 1.0f);
}

float random_neg() {
  return rand() / (static_cast<float>(RAND_MAX) + 1.0f) * 2.0f - 1.0f;
}

GLuint load_texture_2d(const std::filesystem::path filename) {
  int width, height, channels;
  unsigned char *data = stbi_load(filename.generic_string().data(), &width,
                                  &height, &channels, 4);

  GLuint texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);

  glTextureStorage2D(texture, (GLsizei)std::log2(width), GL_RGBA8, width,
                     height);

  glTextureSubImage2D(texture,
                      0,                         //
                      0, 0,                      //
                      width, height,             //
                      GL_RGBA, GL_UNSIGNED_BYTE, //
                      data);

  stbi_image_free(data);

  glGenerateTextureMipmap(texture);

  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
}

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height,
                         std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {

  // --------------------------------------------------------------------------
  // Initialize UBO Data
  // --------------------------------------------------------------------------
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
  camera_ubo.view =
      lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f),
             glm::vec3(0.0f, 1.0f, 0.0f));

  light_ubo.position = glm::vec4(0.0f, 3.0f, 2.0f, 1.0f);
  light_ubo.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light_ubo.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light_ubo.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  object_ubo.model_matrix = glm::mat4(1.0f);
  object_ubo.ambient_color = glm::vec4(0.0f);
  object_ubo.diffuse_color = glm::vec4(1.0f);
  object_ubo.specular_color = glm::vec4(0.0f);

  // Scattered lights.
  for (size_t i = 0; i < 64; i++) {
    lights.push_back({
        glm::vec4(random_neg() * 100.0f, random_neg() * 100.0f,
                  random_neg() * 100.0f, 1.0f), // position
        glm::vec4(0.0f),                        // ambient
        glm::vec4(random_pos() * 10.0, random_pos() * 10.0, random_pos() * 10.0,
                  1.0f), // diffuse
        glm::vec4(0.0f)  // specular
    });
  }

  // Scattered instanced objects.
  const glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(10.0));
  for (size_t i = 0; i < 512; i++) {
    glm::mat4 translate = glm::translate(
        glm::mat4(1.0),
        glm::vec3(random_neg(), random_neg(), random_neg()) * 100.0f);
    instanced_objects.push_back({
        glm::scale(translate, glm::vec3(2.5)), // position
        glm::vec4(0.0f),                       // ambient
        glm::vec4(1.0f),                       // diffuse
        glm::vec4(0.0f)                        // specular
    });
  }

  // --------------------------------------------------------------------------
  // CREATE BUFFERS
  // --------------------------------------------------------------------------

  // Task 8.1: Create buffers for camera, light, and object.
  // Task 8.2/8.3: Create buffer for many lights.
  // Task 8.4: Create buffer for instanced objects.

  // Configure fixed function pipeline
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  compile_shaders();
}

Application::~Application() {
  delete_shaders();

  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_buffer);
  glDeleteBuffers(1, &object_buffer);
  glDeleteBuffers(1, &lights_buffer);
  glDeleteBuffers(1, &instanced_objects_buffer);
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() { glDeleteProgram(main_program); }

void Application::compile_shaders() {
  delete_shaders();

  main_program = create_program(lecture_shaders_path / "main.vert",
                                lecture_shaders_path / "main.frag");
  draw_light_program = create_program(lecture_shaders_path / "draw_light.vert",
                                      lecture_shaders_path / "draw_light.frag");

  std::cout << "Shaders are reloaded." << std::endl;
}

void Application::update(float delta) {}

void Application::render() {
  // --------------------------------------------------------------------------
  // Update UBOs
  // --------------------------------------------------------------------------
  // Camera
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
  camera_ubo.view =
      lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f),
             glm::vec3(0.0f, 1.0f, 0.0f));

  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

  // --------------------------------------------------------------------------
  // Draw scene
  // --------------------------------------------------------------------------
  glViewport(0, 0, width, height);

  // Clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Configure fixed function pipeline
  glEnable(GL_DEPTH_TEST);

  // Draw objects
  glUseProgram(main_program);

  // Task 8.1: Bind the buffers using glBindBufferBase and draw any object.
  // Task 8.2/8.3: Bind buffer with all the lights.
  // Task 8.3: Use draw_lights_program.
  // Task 8.3: Call glDrawElementsInstanced to draw all the lights as spheres
  // (but you can make them teapots if you want (-: ) Task 8.4: Draw instanced
  // objects.
}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

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
