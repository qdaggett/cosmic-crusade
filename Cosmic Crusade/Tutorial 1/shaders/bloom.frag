#version 420

// Remember to bind the two textures to different texture units!
layout(binding = 0) uniform sampler2D u_bright; // bright pass image
layout(binding = 1) uniform sampler2D u_scene; // original scene image

// Fragment Shader Inputs
in	vec3 normal;
in	vec3 texCoord;
in	vec3 posEye;

layout(location = 0) out vec4 FragColor;

void main()
{
	vec4 c1 = texture(u_bright, texCoord.xy);
	vec4 c2 = texture(u_scene, texCoord.xy);

	FragColor = c1 + c2;
}