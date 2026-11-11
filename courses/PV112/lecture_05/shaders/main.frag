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

layout(location = 0) uniform vec3 color;
layout(location = 4) uniform vec3 material_diffuse_color;
layout(location = 5) uniform vec4 light_position;
layout(location = 6) uniform vec3 light_diffuse_color;
layout(location = 7) uniform vec3 eye_position;

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

	// 1. Normalize vectors
	vec3 N = normalize(fs_normal);
	vec3 E = normalize(eye_position - fs_position);

	// 2. Compute Light vector (L) and attenuation based on light type (.w component)
	vec3 L;
	float attenuation = 1.0;

	if (light_position.w == 0.0) {
		// Directional light
		L = normalize(light_position.xyz);
	} else {
		// Point light
		vec3 light_dir = light_position.xyz - fs_position;
		float distance = length(light_dir);
		L = normalize(light_dir);

		// Attenuation calculation
		float constant = 1.0;
		float linear = 0.09;
		float quadratic = 0.032;
		attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
	}

	// 3. Compute Halfway vector (H) for Blinn-Phong
	vec3 H = normalize(L + E);

	// 4. Dot products
	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	// 5. Lighting components
	vec3 ambient = vec3(0.1) * material_diffuse_color;
	vec3 diffuse = light_diffuse_color * material_diffuse_color * NdotL;

	float shininess = 32.0;
	vec3 specular = vec3(1.0) * vec3(1.0) * pow(NdotH, shininess);

	// 6. Final color combined with attenuation
	vec3 lighting = ambient + attenuation * (diffuse + specular);
	final_color = vec4(lighting, 1.0);
}
