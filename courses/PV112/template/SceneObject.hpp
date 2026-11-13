#pragma once
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <filesystem>

class SceneObject {
private:
  GLuint mesh;
  GLuint texture;

public:
  void draw();
};

GLuint load_texture(std::filesystem::path file);