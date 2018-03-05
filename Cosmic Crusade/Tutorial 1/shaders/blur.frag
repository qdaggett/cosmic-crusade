#version 420

layout(binding = 0) uniform sampler2D u_bright; // bright pass image

// (1.0 / windowWidth, 1.0 / windowHeight)
uniform vec4 u_texelSize; // Remember to set this!

// Fragment Shader Inputs

in	vec3 normal;
in	vec3 texCoord;
in	vec3 position;


layout(location = 0) out vec4 FragColor;

vec3 blurred(sampler2D tex, vec2 offsetCoordinates[9], float kernel[9])
{
	vec3 g = vec3(0.0);

	for(int i = 0; i < 9; i++)
	{
			g += texture(tex, offsetCoordinates[i]).rgb * kernel[i];
	}

	return g;
}

void main()
{
	vec2 uv = (texCoord).xy;

	vec2 boxCoordinates[9];
	float kernel[9];
	
	//Top Row
	boxCoordinates[0] = vec2(-u_texelSize.x, u_texelSize.y) + uv;	//Top Left
	boxCoordinates[1] = vec2(0.0, u_texelSize.y) + uv;				//Top Middle
	boxCoordinates[2] = vec2(u_texelSize.x, u_texelSize.y) + uv;	//Top Right

	//Middle Row
	boxCoordinates[3] = vec2(-u_texelSize.x, 0.0) + uv;				//Middle Left
	boxCoordinates[4] = vec2(0.0, 0.0) + uv;						//Middle
	boxCoordinates[5] = vec2(u_texelSize.x, 0.0) + uv;				//Middle Right

	//Bottom Row
	boxCoordinates[6] = vec2(-u_texelSize.x, -u_texelSize.y) + uv;	//Bottom Left
	boxCoordinates[7] = vec2(0.0, -u_texelSize.y) + uv;				//Bottom Middle
	boxCoordinates[8] = vec2(u_texelSize.x, -u_texelSize.y) + uv;	//Bottom Right

	kernel[0] = 0.077847f;
	kernel[1] = 0.123317f;
	kernel[2] = 0.077847f;
	kernel[3] = 0.123317f;
	kernel[4] = 0.195346f;
	kernel[5] = 0.123317f;
	kernel[6] = 0.077847f;
	kernel[7] = 0.123317f;
	kernel[8] = 0.077847f;

	FragColor = vec4(blurred(u_bright, boxCoordinates, kernel), 1.0);
}