#version 420

// Note: Uniform bindings
// This lets you specify the texture unit directly in the shader!
layout(binding = 0) uniform sampler2D u_rgb; // rgb texture

// Fragment Shader Inputs
in vec3 normal;
in vec3 texCoord;
in vec3 position;

// Multiple render targets!
layout(location = 0) out vec4 FragColor;

void main()
{
	vec4 color = texture(u_rgb, texCoord.xy);
	FragColor = vec4(color.xyz, 1.0);
}