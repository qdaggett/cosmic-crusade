#version 420

// Remember to bind the two textures to different texture units!
layout(binding = 0) uniform sampler2D u_top; // bright pass image
layout(binding = 1) uniform sampler2D u_bot; // original scene image

// Fragment Shader Inputs
in	vec3 normal;
in	vec3 texCoord;
in	vec3 posEye;

layout(location = 0) out vec4 FragColor;

void main()
{
	vec4 c1 = texture(u_top, texCoord.xy);
	vec4 c2 = texture(u_bot, texCoord.xy);

	if(c1.a > 0)
		FragColor = c1;

	else
		FragColor = c2;
}