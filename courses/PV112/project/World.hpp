#pragma once

#include "PlayerCamera.hpp"
#include "SceneObject.hpp"
#include "enums.hpp"
#include <filesystem>
#include <memory>
#include <vector>

class World {
private:
  float tick_interval = 0.25f;
  float accumulator = 0.0f;
  int tick_count = 0;
  filesystem::path folder_path;

  std::vector<std::unique_ptr<SceneObject>> objects;
  std::vector<std::unique_ptr<SceneObject>> instantiated_objects;

  std::unique_ptr<PlayerCamera> player;

  glm::vec3 bounds;

  glm::vec3 light_position = glm::vec3(0.0f, 10.0f, 0.0f);

  GameState state = GameState::Menu;

public:
  World(glm::vec3 bounds, std::filesystem::path folder_path);
  void Initialize();

  glm::vec3 GetBounds();
  glm::vec3 GetLightPosition() const;

  std::vector<std::unique_ptr<SceneObject>> &GetObjects();
  std::vector<std::unique_ptr<SceneObject>> &GetInstantiatedObjects();

  GameState GetState() const;
  void SetState(GameState new_state);

  void advance_time(float dt);

  void tick(float fixed_dt);
};