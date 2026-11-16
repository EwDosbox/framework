#pragma once

#include "headers.cpp"

using namespace glm;

class SceneObject {
private:
  GLuint mesh = -1;
  GLuint texture = -1;
  GLuint vbo = -1;

public:
  void draw();
  bool assign_data(std::filesystem::path path);
};

GLuint load_texture(std::filesystem::path file);