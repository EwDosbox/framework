#pragma once
#include <GL/gl.h>
#include <filesystem>

class SceneObject {
private:
  GLuint mesh;
  GLuint texture;

public:
  void draw();
};

GLuint load_texture(std::filesystem::path file);