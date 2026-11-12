#define STB_IMAGE_IMPLEMENTATION
#include "application.hpp"
#include <stb_image.h>

GLuint load_texture(std::filesystem::path file) {
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