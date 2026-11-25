#pragma once

#include "geometry.hpp"
#include "structs.hpp"

#include <filesystem>
#include <memory>

class World;

class SceneObject {
#pragma region Private
private:
  World *world;
  std::unique_ptr<Geometry> mesh_data;

  glm::vec3 position = vec3();
  glm::vec3 rotation = vec3();
  glm::vec3 scale = vec3(1);

  Material material;
  GLuint texture = 0;

  bool is_instanced = false;

  std::vector<glm::mat4> instance_models;
  GLuint instance_ssbo = 0;
  static const GLuint instance_binding = 1;

  static GLuint shader_program;

  glm::mat4 compute_model_matrix() const;
  void upload_instance_data();
#pragma endregion
#pragma region Public
public:
  glm::vec3 GetPosition();
  glm::vec3 GetRotation();
  glm::vec3 GetScale();

  bool SetPosition(glm::vec3 newPosition);
  bool SetPosition(float x, float y, float z);

  bool SetRotation(glm::vec3 newRotation);
  bool SetRotation(float x, float y, float z);

  bool SetScale(glm::vec3 newScale);
  bool SetScale(float x, float y, float z);

  void SetMaterial(const Material &new_material);
  void SetTexture(GLuint texture_id);

  SceneObject(World *world, std::filesystem::path path_to_mesh);
  SceneObject(World *world, std::filesystem::path path_to_mesh,
              std::vector<glm::mat4> instances);

  static void InitShaders(std::filesystem::path shaders_path);
  static void DeleteShaders();

  void draw(const glm::vec3 &light_position);
  void draw_instantiated(const glm::vec3 &light_position);

  void add_instance(const glm::mat4 &model);
  int get_instance_count() const;
};
#pragma endregion
