#version 420

#define NUM_POINT_LIGHTS 1

struct PointLight
{
	vec4 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;

	vec3 hue;

	float specularExponent;
};

uniform PointLight pointLight;

uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform Material material;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

vec3 rim(PointLight light)
{
	vec3 N = normalize(normal);
	vec3 v = normalize(light.position.xyz);
	float vDotn = max(dot(v,N), 0.0);

	return vec3(smoothstep(0,.5,vDotn));
}

void main()
{
	outColor = vec4(1);
}



