#pragma once

#include "World.hpp"
#include "geometry.hpp"
#include "headers.cpp"
#include <filesystem>
#include <glm/fwd.hpp>
#include <memory>

using namespace glm;
using namespace std;

class SceneObject {
#pragma region Private
private:
  World *world;
  shared_ptr<Geometry> mesh_data;

  vec3 position = vec3();
  vec3 rotation = vec3();
  vec3 scale = vec3();

  GLuint mesh_id = -1;
  GLuint texture = -1;
  GLuint vbo = -1;
#pragma endregion
#pragma region Public

public:
  vec3 GetPosition();
  vec3 GetRotation();
  vec3 GetScale();

  bool SetPosition(vec3 newPosition);
  bool SetPosition(float x, float y, float z);

  bool SetRotation(vec3 newRotation);
  bool SetRotation(float x, float y, float z);

  bool SetScale(vec3 newScale);
  bool SetScale(float x, float y, float z);

  SceneObject(World *World, std::filesystem::path path_to_mesh);

  void draw();
  bool assign_data(std::filesystem::path path);
};
#pragma endregion
