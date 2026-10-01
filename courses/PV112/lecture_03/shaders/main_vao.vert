#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;
// The color of the current vertex that is being processed.
layout(location = 1) in vec3 color;

layout(location = 0) out vec4 out_color;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// Task 3.8: Define the output variable for color.

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
    // Task 3.8: Output color to fragment shader (set the output value).
    gl_Position = vec4(position, 1.0);
    out_color = vec4(color, 1.0);
}