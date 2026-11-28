#pragma once

#include "geometry.hpp"
#include "structs.hpp"

#include <glm/glm.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

class World;

class SceneObject {
#pragma region Private
private:
  World *pWorld;
  std::unique_ptr<Geometry> pMeshData;

  glm::vec3 position = vec3();
  glm::vec3 rotation = vec3();
  glm::vec3 scale = vec3(1);

  Material material;
  GLuint texture = 0;

  bool isInstanced = false;

  std::vector<glm::mat4> pInstanceModels;
  GLuint SSBO = 0;
  static constexpr GLuint instanceBinding = 1;

  static GLuint shaderProgram;

  glm::mat4 ComputeModelMatrix() const;
  void UploadInstanceData();
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

  void Draw(const glm::vec3 &light_position);
  void DrawInstances(const glm::vec3 &light_position);

  void AddInstance(const glm::mat4 &model);
  int GetInstanceCount() const;
};
#pragma endregion
