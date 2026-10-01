#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
// There are no custom input variables (except the in-build ones).

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
// There are no custom output variables (except the in-build ones).

// ----------------------------------------------------------------------------
// Local Variables
// ----------------------------------------------------------------------------
// Task 3.1: Modify the vertices to draw a square as two triangles.
// Task 3.2: Modify the vertices to draw a square using GL_TRIANGL_STRIP.
const vec2 triangle[6] = {
	vec2(-0.5, -0.5),
	vec2(0.5, -0.5),
	vec2(-0.5, 0.5),
	vec2(0.5, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
};
layout (location = 0) out vec3 color;
// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	color = gl_VertexID < 3 ? vec3(1,0,0) : vec3(0,1,0);
    gl_Position = vec4(triangle[gl_VertexID], 0.0, 1.0);
}