#version 420

layout(binding = 0) uniform sampler2D u_scene; // original scene image

uniform float u_bloomThreshold;

// Fragment Shader Inputs

in	vec3 normal;
in	vec3 texCoord;
in	vec3 position;

layout(location = 0) out vec4 FragColor;

void main()
{
	vec4 c = texture(u_scene, texCoord.xy);

	FragColor = clamp((c - u_bloomThreshold) / (1 - u_bloomThreshold), 0, 1);
}