#include "SceneObject.hpp"

#include "Utility.hpp"
#include "World.hpp"
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#pragma region Static

GLuint SceneObject::shaderProgram = 0;

#pragma endregion
#pragma region Private

mat4 SceneObject::ComputeModelMatrix() const {
  mat4 model = translate(mat4(1.0f), position);
  model = rotate(model, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
  model = rotate(model, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
  model = rotate(model, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);
  return model;
}

void SceneObject::UploadInstanceData() {
  if (SSBO == 0) {
    glCreateBuffers(1, &SSBO);
  }
  glNamedBufferData(
      SSBO, static_cast<GLsizeiptr>(pInstanceModels.size() * sizeof(mat4)),
      pInstanceModels.data(), GL_DYNAMIC_DRAW);
}

#pragma endregion
#pragma region Constructors and Destructors

SceneObject::SceneObject(World *world, std::filesystem::path path_to_mesh)
    : pWorld(world),
      pMeshData(make_unique<Geometry>(Geometry::from_file(path_to_mesh))) {
  isInstanced = false;
  pInstanceModels = {ComputeModelMatrix()};
  UploadInstanceData();
}

SceneObject::SceneObject(World *world, std::filesystem::path path_to_mesh,
                         std::vector<glm::mat4> instances)
    : pWorld(world),
      pMeshData(make_unique<Geometry>(Geometry::from_file(path_to_mesh))),
      pInstanceModels(std::move(instances)) {
  isInstanced = true;
  if (pInstanceModels.empty()) {
    pInstanceModels.push_back(ComputeModelMatrix());
  }
  UploadInstanceData();
}

#pragma endregion
#pragma region Getters and Setters

vec3 SceneObject::GetPosition() { return this->position; }
vec3 SceneObject::GetRotation() { return this->rotation; }
vec3 SceneObject::GetScale() { return this->scale; }

bool SceneObject::SetPosition(vec3 newPosition) {
  if (Utility::is_contained(newPosition, vec3(), this->pWorld->GetBounds())) {
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
void SceneObject::SetMaterial(const Material &new_material) {
  this->material = new_material;
}
void SceneObject::SetTexture(GLuint texture_id) { this->texture = texture_id; }

#pragma region Methods

void SceneObject::InitShaders(filesystem::path shaders_path) {
  GLuint vertex_shader =
      Utility::compile_shader(shaders_path / "object.vert", GL_VERTEX_SHADER);
  GLuint fragment_shader =
      Utility::compile_shader(shaders_path / "object.frag", GL_FRAGMENT_SHADER);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertex_shader);
  glAttachShader(shaderProgram, fragment_shader);
  glLinkProgram(shaderProgram);

  Utility::delete_shader(vertex_shader, shaderProgram);
  Utility::delete_shader(fragment_shader, shaderProgram);

  GLint success = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char log[1024];
    glGetProgramInfoLog(shaderProgram, sizeof(log), nullptr, log);
    std::cerr << "Shader link error:\n" << log << std::endl;
  }
}

void SceneObject::DeleteShaders() {
  if (shaderProgram != 0) {
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
  }
}

void SceneObject::Draw(const vec3 &light_position) {
  pInstanceModels[0] = ComputeModelMatrix();
  UploadInstanceData();
  DrawInstances(light_position);
}

void SceneObject::DrawInstances(const vec3 &light_position) {
  if (shaderProgram == 0 || pInstanceModels.empty() || !pMeshData) {
    return;
  }

  glUseProgram(shaderProgram);

  glUniform3fv(glGetUniformLocation(shaderProgram, "light_position"), 1,
               value_ptr(light_position));

  glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1,
               value_ptr(material.ambient_color));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1,
               value_ptr(material.diffuse_color));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1,
               value_ptr(material.specular_color));
  glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"),
              material.shininess);

  glUniform1i(glGetUniformLocation(shaderProgram, "has_texture"),
              texture != 0 ? 1 : 0);
  if (texture != 0) {
    glBindTextureUnit(0, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "object_texture"), 0);
  }

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, instanceBinding, SSBO);

  pMeshData->bind_vao();
  pMeshData->draw_instanced(static_cast<int>(pInstanceModels.size()));
  glBindVertexArray(0);
}

void SceneObject::AddInstance(const mat4 &model) {
  pInstanceModels.push_back(model);
  isInstanced = true;
  UploadInstanceData();
}

int SceneObject::GetInstanceCount() const {
  return static_cast<int>(pInstanceModels.size());
}

#pragma endregion