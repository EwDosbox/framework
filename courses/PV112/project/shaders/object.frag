#version 460 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 v_world_pos;
in vec3 v_normal;
in vec2 v_uv;

uniform vec3 light_position;
uniform Material material;
uniform int has_texture;
uniform sampler2D object_texture;

out vec4 frag_color;

void main() {
	vec3 N = normalize(v_normal);
	vec3 L = normalize(light_position - v_world_pos);
	float diff = max(dot(N, L), 0.15);

	vec3 base_color = material.diffuse;
	if (has_texture != 0) {
		base_color *= texture(object_texture, v_uv).rgb;
	}

	frag_color = vec4(base_color * diff, 1.0);
}
