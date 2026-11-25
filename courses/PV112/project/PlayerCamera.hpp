#pragma once

#include "glad/glad.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class PlayerCamera {
private:
  vec3 position{0.0f, 1.5f, 8.0f};
  float yaw_deg = -90.0f;
  float pitch_deg = 0.0f;
  float move_speed = 6.0f;
  float turn_speed = 90.0f;

  float fov_deg = 60.0f;
  float aspect_ratio = 16.0f / 9.0f;
  float near_plane = 0.1f;
  float far_plane = 100.0f;

  GLuint camera_ubo = 0;
  static const GLuint ubo_binding = 0;

public:
  void set_position(const vec3 &pos);
  void move_world(const vec3 &delta);
  void move_forward(float amount);
  void move_right(float amount);

  void set_rotation(float yaw, float pitch = 0.0f);
  void rotate(float delta_yaw, float delta_pitch = 0.0f);
  void turn_left(float dt);
  void turn_right(float dt);

  void set_speeds(float move, float turn);
  void set_projection(float fov_degrees, float aspect, float near_p,
                      float far_p);

  vec3 get_position() const { return position; }
  float get_yaw() const { return yaw_deg; }
  float get_pitch() const { return pitch_deg; }

  vec3 get_forward() const;
  vec3 get_flat_forward() const;
  vec3 get_right() const;
  mat4 get_view_matrix() const;
  mat4 get_projection_matrix() const;

  void init_gpu();
  void update_gpu() const;
  void bind_gpu() const;
  GLuint get_ubo_binding() const { return ubo_binding; }
};