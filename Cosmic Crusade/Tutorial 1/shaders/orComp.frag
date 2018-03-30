#version 420

// Remember to bind the two textures to different texture units!
layout(binding = 0) uniform sampler2D u_top; 
layout(binding = 1) uniform sampler2D u_bot; 

// Fragment Shader Inputs
in	vec3 normal;
in	vec3 texCoord;
in	vec3 posEye;

uniform bool u_alpha;

layout(location = 0) out vec4 FragColor;

void main()
{
	vec4 c1 = texture(u_top, texCoord.xy);
	vec4 c2 = texture(u_bot, texCoord.xy);

	if(!u_alpha)
	{
		if((c1.r > 0) || (c1.g > 0) || (c1.b > 0))
			FragColor = c1;

		else
			FragColor = c2;
	}

	else
	{
		if(c1.a > 0)
			FragColor = c1;

		else
			FragColor = c2;
	}
}