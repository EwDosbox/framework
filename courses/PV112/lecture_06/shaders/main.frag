#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// The eye(camera) position.
layout(location = 3) uniform vec3 eye_position;

// The light's position and ambient+diffuse+specular terms.
layout(location = 4) uniform vec4 light_position;
layout(location = 5) uniform vec3 light_ambient_color;
layout(location = 6) uniform vec3 light_diffuse_color;
layout(location = 7) uniform vec3 light_specular_color;

// The material's ambient+diffuse+specular+shininess terms.
layout(location = 8) uniform vec3 material_ambient_color;
layout(location = 9) uniform vec3 material_diffuse_color;
layout(location = 10) uniform vec3 material_specular_color;
layout(location = 11) uniform float material_shininess;

// The intpolated position from the vertex shader.
layout(location = 0) in vec3 fs_position;
// The intpolated normal from the vertex shader.
layout(location = 1) in vec3 fs_normal;
// The intpolated texture coordinates from the vertex shader.
layout(location = 2) in vec2 fs_texture_coordinate;

// Task 6.2: Add a sampler binding for texture.
//           layout(binding = NUMBER) uniform sampler2D NAME;
layout(binding = 0) uniform sampler2D diffuse_texture;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The final output color.
layout(location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// Task 6.2: Fetch color from diffuse texture using fs_texture_coordinate and set it as the final color.
	//           The function is: texture(sampler, texture_coordinate) and returns vec4
	vec4 texture_color = texture(diffuse_texture, fs_texture_coordinate);

	vec3 light_vector = light_position.xyz - fs_position * light_position.w;
	vec3 L = normalize(light_vector);
	vec3 N = normalize(fs_normal);
	vec3 E = normalize(eye_position - fs_position);
	vec3 H = normalize(L + E);

	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	// Task 6.3: Multiply the material diffuse color by the color obtained from the diffuse texture.

	vec3 surface_diffuse = material_diffuse_color * texture_color.rgb;
	vec3 surface_ambient = material_ambient_color * texture_color.rgb;

	vec3 material_ambient = surface_ambient * light_ambient_color;
	vec3 material_diffuse = surface_diffuse * light_diffuse_color * NdotL;
	vec3 material_specular = material_specular_color * light_specular_color * pow(NdotH, material_shininess);

	float distance2 = light_position.w == 1.0 ? dot(light_vector, light_vector) : 1.0;

	vec3 material_color = material_ambient + (material_diffuse + material_specular) / distance2;

	final_color = vec4(material_color , texture_color.a);
}
