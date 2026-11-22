#pragma once
#include "headers.cpp"
using namespace glm;

class Utility {
private:
public:
  static std::string load_file(std::filesystem::path path);
  static GLuint compile_shader(std::filesystem::path path, GLenum type);
  static void delete_shader(GLuint shader, GLuint program);
  static GLuint load_texture(std::filesystem::path file);
  static bool is_contained(vec3 actual, vec3 min, vec3 max);
  static bool is_contained(int actual, int min, int max);
};