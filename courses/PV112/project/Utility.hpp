#pragma once
#include "headers.cpp"

class Utility {
private:
public:
  static std::string load_file(std::filesystem::path path);
  static GLuint compile_shader(std::filesystem::path path, GLenum type);
  static void delete_shader(GLuint shader, GLuint program);
};