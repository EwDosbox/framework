#include "World.hpp"
#include "PlayerCamera.hpp"
#include "SceneObject.hpp"
#include <filesystem>
#include <glm/fwd.hpp>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GL/gl.h>

using namespace glm;
using namespace std;

World::World(vec3 bounds, filesystem::path lecture_path) {
  this->bounds = bounds;
  this->folder_path = lecture_path;
}

void World::Initialize() {
  state = GameState::InGame;

  this->player = make_unique<PlayerCamera>();
  player->init_gpu();
  player->update_gpu();
  player->bind_gpu();

  filesystem::path mesh_dir = this->folder_path / "objects";

  auto ground = make_unique<SceneObject>(this, mesh_dir / "plane.obj");
  ground->SetPosition(bounds.x / 2.0f, 0.0f, bounds.z / 2.0f);
  ground->SetScale(bounds.x, 1.0f, bounds.z);
  Material ground_material;
  ground_material.diffuse_color = vec3(0.3f, 0.3f, 0.3f);
  ground_material.shininess = 8.0f;
  ground->SetMaterial(ground_material);
  objects.push_back(std::move(ground));

  auto hero_bunny = make_unique<SceneObject>(this, mesh_dir / "bunny.obj");
  hero_bunny->SetPosition(bounds.x / 2.0f, 0.0f, bounds.z / 2.0f - 3.0f);
  Material hero_material;
  hero_material.diffuse_color = vec3(0.8f, 0.2f, 0.2f);
  hero_bunny->SetMaterial(hero_material);
  objects.push_back(std::move(hero_bunny));

  const int crowd_size = 5;
  vector<mat4> bunny_crowd_transforms;
  bunny_crowd_transforms.reserve(crowd_size);
  for (int i = 0; i < crowd_size; i++) {
    float t = crowd_size > 1 ? static_cast<float>(i) / (crowd_size - 1) : 0.0f;
    float x = 2.0f + t * (bounds.x - 4.0f);
    mat4 model = translate(mat4(1.0f), vec3(x, 0.0f, 2.0f));
    bunny_crowd_transforms.push_back(model);
  }
  auto bunny_crowd = make_unique<SceneObject>(
      this, mesh_dir / "bunny.obj", std::move(bunny_crowd_transforms));
  Material crowd_material;
  crowd_material.diffuse_color = vec3(0.6f, 0.6f, 0.2f);
  bunny_crowd->SetMaterial(crowd_material);
  instantiated_objects.push_back(std::move(bunny_crowd));
}

vec3 World::GetBounds() { return this->bounds; }
vec3 World::GetLightPosition() const { return this->light_position; }

std::vector<std::unique_ptr<SceneObject>> &World::GetObjects() {
  return objects;
}
std::vector<std::unique_ptr<SceneObject>> &World::GetInstantiatedObjects() {
  return instantiated_objects;
}

GameState World::GetState() const { return state; }
void World::SetState(GameState new_state) { state = new_state; }

void World::advance_time(float dt) {
  if (state != GameState::InGame) {
    return;
  }

  accumulator += dt;
  while (accumulator >= tick_interval) {
    accumulator -= tick_interval;
    tick(tick_interval);
  }
}

void World::tick(float fixed_dt) { tick_count++; }