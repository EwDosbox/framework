#version 460 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

layout(std140, binding = 0) uniform CameraBlock {
    mat4 projection;
    mat4 view;
    vec4 position;
} camera;

layout(std430, binding = 1) buffer InstanceMatrices {
    mat4 instance_models[];
};

out vec3 v_world_pos;
out vec3 v_normal;
out vec2 v_uv;

void main() {
    mat4 model = instance_models[gl_InstanceID];
    vec4 world_pos = model * vec4(a_pos, 1.0);

    v_world_pos = world_pos.xyz;
    v_normal = normalize(mat3(transpose(inverse(model))) * a_normal);
    v_uv = a_uv;

    gl_Position = camera.projection * camera.view * world_pos;
}
