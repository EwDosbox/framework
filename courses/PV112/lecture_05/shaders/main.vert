#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// Task 5.1: Add uniform variables for projection, view, model matrices.

// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;
// The normal of the current vertex that is being processed.
layout(location = 1) in vec3 normal;
// The texture coordinates of the current vertex that is being processed.
layout(location = 2) in vec2 texture_coordinate;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The position forwarded to fragment shader.
layout(location = 0) out vec3 fs_position;
// The normal forwarded to fragment shader.
layout(location = 1) out vec3 fs_normal;
// The texture coordinates forwarded to fragment shader.
layout(location = 2) out vec2 fs_texture_coordinate;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// Task 5.1 Transform position to world space when you obtain model matrix.
	fs_position = position;
	// Task 5.1 Uncomment when you obtain model matrix.
	// fs_normal = transpose(inverse(mat3(model))) * normal;
	fs_texture_coordinate = texture_coordinate;

	// Task 5.1: Multiply position by MVP matrices in correct order
    gl_Position = vec4(position, 1.0);
}
