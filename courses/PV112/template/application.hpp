// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include "camera.h"
#include "cube.hpp"
#include "pv112_application.hpp"
#include "sphere.hpp"
#include "teapot.hpp"

// ----------------------------------------------------------------------------
// UNIFORM STRUCTS
// ----------------------------------------------------------------------------
struct CameraUBO {
  glm::mat4 projection;
  glm::mat4 view;
  glm::vec4 position;
};

struct LightUBO {
  glm::vec4 position;
  glm::vec4 ambient_color;
  glm::vec4 diffuse_color;
  glm::vec4 specular_color;
};

// The C++ alignas specifier may be applied to type definitions to specify a
// memory alignment for instances of that type. This is useful for UBOs, where
// each element must be aligned to 256 bytes (or more precisely to multiple of
// GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT). This forces the compiler to align
// elements of std::vector<ObjectUBO> to 256 bytes despite them being smaller.
// Note that if you decide to change the alignment, you must also change the
// offset in glBindBufferRange calls and ensure it will work on Windows.
struct alignas(256) ObjectUBO {
  glm::mat4 model_matrix;  // [  0 -  64) bytes
  glm::vec4 ambient_color; // [ 64 -  80) bytes
  glm::vec4 diffuse_color; // [ 80 -  96) bytes
  // Contains shininess in .w element
  glm::vec4 specular_color; // [ 96 - 112) bytes
};

class Application : public PV112Application {
  // ----------------------------------------------------------------------------
  // Variables
  // ----------------------------------------------------------------------------

  // Paths
  std::filesystem::path images_path;
  std::filesystem::path objects_path;

  // Main program
  GLuint main_program;
  // TODO: feel free to add as many as you need/like

  // List of geometries used in the project
  std::vector<std::shared_ptr<Geometry>> geometries;
  // Shared pointers are pointers that automatically count how many times they
  // are used. When there are 0 pointers to the object pointed by shared_ptrs,
  // the object is automatically deallocated. Consequently, we gain 3 main
  // properties:
  // 1. Objects are not unnecessarily copied
  // 2. We don't have to track pointers
  // 3. We don't have to deallocate these geometries
  std::shared_ptr<Geometry> sphere;
  std::shared_ptr<Geometry> bunny;

  // Default camera that rotates around center.
  Camera camera;

  // UBOs for camera, lights, and objects
  GLuint camera_buffer = 0;
  CameraUBO camera_ubo;

  GLuint light_buffer = 0;
  LightUBO light_ubo;

  GLuint objects_buffer = 0;
  std::vector<ObjectUBO> objects_ubos;

  // Textures
  GLuint marble_texture = 0;

  // ----------------------------------------------------------------------------
  // Constructors & Destructors
  // ----------------------------------------------------------------------------
public:
  /**
   * Constructs a new @link Application with a custom width and height.
   *
   * @param 	initial_width 	The initial width of the window.
   * @param 	initial_height	The initial height of the window.
   * @param 	arguments	  	The command line arguments used to
   * obtain the application directory.
   */
  Application(int initial_width, int initial_height,
              std::vector<std::string> arguments = {});

  /** Destroys the {@link Application} and releases the allocated resources. */
  ~Application() override;

  // ----------------------------------------------------------------------------
  // Methods
  // ----------------------------------------------------------------------------

  /** @copydoc PV112Application::compile_shaders */
  void compile_shaders() override;

  /** @copydoc PV112Application::delete_shaders */
  void delete_shaders() override;

  /** @copydoc PV112Application::update */
  void update(float delta) override;

  /** @copydoc PV112Application::render */
  void render() override;

  /** @copydoc PV112Application::render_ui */
  void render_ui() override;

  // ----------------------------------------------------------------------------
  // Input Events
  // ----------------------------------------------------------------------------

  /** @copydoc PV112Application::on_resize */
  void on_resize(int width, int height) override;

  /** @copydoc PV112Application::on_mouse_move */
  void on_mouse_move(double x, double y) override;

  /** @copydoc PV112Application::on_mouse_button */
  void on_mouse_button(int button, int action, int mods) override;

  /** @copydoc PV112Application::on_key_pressed */
  void on_key_pressed(int key, int scancode, int action, int mods) override;
};
