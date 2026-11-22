#include "SceneObject.hpp"
#include "Utility.hpp"
#include <glm/fwd.hpp>

vec3 SceneObject::GetPosition() { return this->position; }

vec3 SceneObject::GetRotation() { return this->rotation; }
vec3 SceneObject::GetScale() { return this->scale; }

bool SceneObject::SetPosition(vec3 newPosition) {
  if (Utility::is_contained(newPosition, vec3(), this->world->GetBounds())) {
    this->position = newPosition;
    return true;
  } else {
    return false;
  }
}
bool SceneObject::SetPosition(float x, float y, float z) {
  return SetPosition(vec3(x, y, z));
}

bool SceneObject::SetRotation(vec3 newRotation) {
  this->rotation = newRotation;
  return true;
}
bool SceneObject::SetRotation(float x, float y, float z) {
  this->rotation = vec3(x, y, z);
  return true;
}

bool SceneObject::SetScale(vec3 newScale) {
  this->scale = newScale;
  return true;
}
bool SceneObject::SetScale(float x, float y, float z) {
  this->scale = vec3(x, y, z);
  return true;
}

SceneObject::SceneObject(World *World) { this->world = World; }

void SceneObject::draw() { return; }
bool SceneObject::assign_data(std::filesystem::path path) { return true; }
