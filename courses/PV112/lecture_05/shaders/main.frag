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
	// Task 5.2: Compute final color using Blinn–Phong shading model.
	// Task 5.2: Use attenuation based on the light distance for point lights.
	// Task 5.2: Differentiate between directional and point light based on its .w value in position.

	final_color = vec4(1.0, 1.0, 1.0, 1.0);
}
