#pragma once

#include "glad/glad.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <string>

class Utility {
private:
public:
  static std::string load_file(std::filesystem::path path);
  static GLuint compile_shader(std::filesystem::path path, GLenum type);
  static void delete_shader(GLuint shader, GLuint program);
  static GLuint load_texture(std::filesystem::path file);
  static bool is_contained(glm::vec3 actual, glm::vec3 min, glm::vec3 max);
  static bool is_contained(int actual, int min, int max);
};