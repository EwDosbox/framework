#pragma once

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

struct CameraData {
  mat4 view;
  mat4 projection;
  vec4 position;
};

struct Material {
  vec3 ambient_color = vec3(0.1f);
  vec3 diffuse_color = vec3(0.8f);
  vec3 specular_color = vec3(1.0f);
  float shininess = 32.0f;
};
