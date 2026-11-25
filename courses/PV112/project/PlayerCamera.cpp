#include "PlayerCamera.hpp"

#include "structs.hpp"

#define GLFW_INCLUDE_NONE
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

using namespace std;
using namespace glm;

void PlayerCamera::set_position(const vec3 &pos) { position = pos; }

void PlayerCamera::move_world(const vec3 &delta) { position += delta; }

void PlayerCamera::move_forward(float amount) {
  position += get_flat_forward() * amount;
}

void PlayerCamera::move_right(float amount) {
  position += get_right() * amount;
}

void PlayerCamera::set_rotation(float yaw, float pitch) {
  yaw_deg = yaw;
  pitch_deg = std::clamp(pitch, -89.0f, 89.0f);
}

void PlayerCamera::rotate(float delta_yaw, float delta_pitch) {
  set_rotation(yaw_deg + delta_yaw, pitch_deg + delta_pitch);
}

void PlayerCamera::turn_left(float dt) { rotate(-turn_speed * dt, 0.0f); }

void PlayerCamera::turn_right(float dt) { rotate(turn_speed * dt, 0.0f); }

void PlayerCamera::set_speeds(float move, float turn) {
  move_speed = move;
  turn_speed = turn;
}

void PlayerCamera::set_projection(float fov_degrees, float aspect, float near_p,
                                  float far_p) {
  fov_deg = fov_degrees;
  aspect_ratio = aspect;
  near_plane = near_p;
  far_plane = far_p;
}

glm::vec3 PlayerCamera::get_forward() const {
  float yaw_rad = radians(yaw_deg);
  float pitch_rad = radians(pitch_deg);

  vec3 dir;
  dir.x = cos(yaw_rad) * cos(pitch_rad);
  dir.y = sin(pitch_rad);
  dir.z = sin(yaw_rad) * cos(pitch_rad);
  return normalize(dir);
}

glm::vec3 PlayerCamera::get_flat_forward() const {
  float yaw_rad = radians(yaw_deg);
  return normalize(vec3(cos(yaw_rad), 0.0f, sin(yaw_rad)));
}

glm::vec3 PlayerCamera::get_right() const {
  return normalize(cross(get_flat_forward(), vec3(0.0f, 1.0f, 0.0f)));
}

glm::mat4 PlayerCamera::get_view_matrix() const {
  return lookAt(position, position + get_forward(), vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 PlayerCamera::get_projection_matrix() const {
  return perspective(radians(fov_deg), aspect_ratio, near_plane, far_plane);
}

void PlayerCamera::init_gpu() {
  glCreateBuffers(1, &camera_ubo);
  glNamedBufferData(camera_ubo, sizeof(CameraData), nullptr, GL_DYNAMIC_DRAW);
}

void PlayerCamera::update_gpu() const {
  CameraData data;
  data.view = get_view_matrix();
  data.projection = get_projection_matrix();
  data.position = vec4(position, 1.0f);
  glNamedBufferSubData(camera_ubo, 0, sizeof(CameraData), &data);
}

void PlayerCamera::bind_gpu() const {
  glBindBufferBase(GL_UNIFORM_BUFFER, ubo_binding, camera_ubo);
}