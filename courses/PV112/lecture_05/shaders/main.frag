#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// Task 5.2: Add a uniform variable for eye(camera) position.
// Task 5.2: Add a uniform variable for light's position and ambient+diffuse+specular terms.
// Task 5.2: Add a uniform variable for material's ambient+diffuse+specular+shininess terms.

// The intpolated position from the vertex shader.
layout(location = 0) in vec3 fs_position;
// The intpolated normal from the vertex shader.
layout(location = 1) in vec3 fs_normal;
// The intpolated texture coordinates from the vertex shader.
layout(location = 2) in vec2 fs_texture_coordinate;

//layout(location = 3) in vec3 N;

layout(location = 0) uniform vec3 color;
layout(location = 4) uniform vec3 material_diffuse_color;
layout(location = 5) uniform vec4 light_position;
layout(location = 6) uniform vec3 light_diffuse_color;

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
	// Task 5.2: Compute (N)ormal, (L)ight, (E)ye, (H)alf vectors.
	// Task 5.2: Compute dot products between N and L, N and H.
	// Task 5.2: Compute material values multiplied by light's values.
	// Task 5.2: Compute final color using Blinn�Phong shading model.
	// Task 5.2: Use attenuation based on the light distance for point lights.
	// Task 5.2: Differentiate between directional and point light based on its .w value in position.

  vec3 L = normalize(light_position.xyz - vec3(0,0,0));	

  vec3 Idiff = light_diffuse_color * material_diffuse_color * max(dot(N, L), 0.0); 
  
  Idiff = clamp(Idiff, 0.0, 1.0);
  final_color = vec4(Idiff, 1.0);
}
