#include "Utility.hpp"
#include <fstream>

std::string Utility::load_file(std::filesystem::path path) {

  std::ifstream file(path);

  std::stringstream string_stream;
  string_stream << file.rdbuf();

  return string_stream.str();
}

GLuint Utility::compile_shader(std::filesystem::path path, GLenum type) {
  GLuint shader = glCreateShader(type);
  std::string source = load_file(path);
  // std::cout << source << std::endl;
  const char *source_char = source.data();
  glShaderSource(shader, 1, &source_char, nullptr);
  glCompileShader(shader);

  return shader;
}

void Utility::delete_shader(GLuint shader, GLuint program) {
  glDetachShader(program, shader);
  glDeleteShader(shader);
}