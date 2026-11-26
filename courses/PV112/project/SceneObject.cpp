#include "SceneObject.hpp"

#include "Utility.hpp"
#include "World.hpp"
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

GLuint SceneObject::shader_program = 0;

#pragma region Private
mat4 SceneObject::compute_model_matrix() const {
  mat4 model = translate(mat4(1.0f), position);
  model = rotate(model, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
  model = rotate(model, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
  model = rotate(model, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);
  return model;
}

void SceneObject::upload_instance_data() {
  if (instance_ssbo == 0) {
    glCreateBuffers(1, &instance_ssbo);
  }
  glNamedBufferData(
      instance_ssbo,
      static_cast<GLsizeiptr>(instance_models.size() * sizeof(mat4)),
      instance_models.data(), GL_DYNAMIC_DRAW);
}
#pragma endregion
#pragma region Public

#pragma region Getters and Setters

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
void SceneObject::SetMaterial(const Material &new_material) {
  this->material = new_material;
}
void SceneObject::SetTexture(GLuint texture_id) { this->texture = texture_id; }

#pragma endregion
#pragma region Constructors

SceneObject::SceneObject(World *world, string name,
                         filesystem::path path_to_mesh)
    : world(world), name(name),
      mesh_data(make_unique<Geometry>(Geometry::from_file(path_to_mesh))) {
  is_instanced = false;
  instance_models = {compute_model_matrix()};
  upload_instance_data();
}

SceneObject::SceneObject(World *world, string name,
                         filesystem::path path_to_mesh,
                         std::vector<glm::mat4> instances)
    : world(world), name(name),
      mesh_data(make_unique<Geometry>(Geometry::from_file(path_to_mesh))),
      instance_models(std::move(instances)) {
  is_instanced = true;
  if (instance_models.empty()) {
    instance_models.push_back(compute_model_matrix());
  }
  upload_instance_data();
}

#pragma endregion
#pragma region Methods

void SceneObject::InitShaders(filesystem::path shaders_path) {
  GLuint vertex_shader =
      Utility::compile_shader(shaders_path / "object.vert", GL_VERTEX_SHADER);
  GLuint fragment_shader =
      Utility::compile_shader(shaders_path / "object.frag", GL_FRAGMENT_SHADER);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  Utility::delete_shader(vertex_shader, shader_program);
  Utility::delete_shader(fragment_shader, shader_program);

  GLint success = 0;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    char log[1024];
    glGetProgramInfoLog(shader_program, sizeof(log), nullptr, log);
    std::cerr << "Shader link error:\n" << log << std::endl;
  }
}

void SceneObject::DeleteShaders() {
  if (shader_program != 0) {
    glDeleteProgram(shader_program);
    shader_program = 0;
  }
}

void SceneObject::draw(const vec3 &light_position) {
  instance_models[0] = compute_model_matrix();
  upload_instance_data();
  draw_instantiated(light_position);
}

void SceneObject::draw_instantiated(const vec3 &light_position) {
  if (shader_program == 0 || instance_models.empty() || !mesh_data) {
    return;
  }

  glUseProgram(shader_program);

  glUniform3fv(glGetUniformLocation(shader_program, "light_position"), 1,
               value_ptr(light_position));

  glUniform3fv(glGetUniformLocation(shader_program, "material.ambient"), 1,
               value_ptr(material.ambient_color));
  glUniform3fv(glGetUniformLocation(shader_program, "material.diffuse"), 1,
               value_ptr(material.diffuse_color));
  glUniform3fv(glGetUniformLocation(shader_program, "material.specular"), 1,
               value_ptr(material.specular_color));
  glUniform1f(glGetUniformLocation(shader_program, "material.shininess"),
              material.shininess);

  glUniform1i(glGetUniformLocation(shader_program, "has_texture"),
              texture != 0 ? 1 : 0);
  if (texture != 0) {
    glBindTextureUnit(0, texture);
    glUniform1i(glGetUniformLocation(shader_program, "object_texture"), 0);
  }

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, instance_binding, instance_ssbo);

  mesh_data->bind_vao();
  mesh_data->draw_instanced(get_instance_count());
  glBindVertexArray(0);
}

void SceneObject::add_instance(const mat4 &model) {
  instance_models.push_back(model);
  is_instanced = true;
  upload_instance_data();
}

int SceneObject::get_instance_count() const {
  return static_cast<int>(instance_models.size());
}

#pragma endregion