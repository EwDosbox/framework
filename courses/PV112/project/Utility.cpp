#include "Utility.hpp"
#include <cmath>
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

GLuint Utility::load_texture(std::filesystem::path file) {
  int width = -1, height = -1, components = -1;
  unsigned char *data = stbi_load(file.string().c_str(), &width, &height,
                                  &components, 4 /*RGBA*/);
  GLuint texture_id = -1;

  if (width > 0) {
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);

    glTextureStorage2D(texture_id, std::log2(width) + 1, GL_RGBA8, width,
                       height);
    glTextureSubImage2D(texture_id, 0, 0, 0, width, height, GL_RGBA,
                        GL_UNSIGNED_BYTE, data);

    glGenerateTextureMipmap(texture_id);
  }

  glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

  stbi_image_free(data);
  return texture_id;
}

bool Utility::is_contained(vec3 actual, vec3 min, vec3 max) {
  if (is_contained(actual.x, min.x, max.x) &&
      is_contained(actual.y, min.y, max.y) &&
      is_contained(actual.z, min.z, max.z)) {
    return true;
  } else {
    return false;
  }
}
bool Utility::is_contained(int actual, int min, int max) {
  if (min < actual && max > actual) {
    return true;
  } else {
    return false;
  }
}