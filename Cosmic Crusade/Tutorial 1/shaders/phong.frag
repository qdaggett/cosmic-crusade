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
//uniform PointLight pointLight1;
//uniform PointLight pointLight2;

uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform Material material;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

vec3 calculatePointLight(PointLight light, vec3 norm, vec4 materialDiffuse, vec4 materialSpecular);

void main()
{
	//account for rasterizer interpolating 
	vec3 norm = normalize(normal);

	vec4 diffuse = texture(material.diffuse, texCoord);
	vec4 specular = texture(material.specular, texCoord);

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		outColor.rgb += calculatePointLight(pointLights[i], norm, diffuse, specular);
	}

	//outColor.rgb += calculatePointLight(pointLight0, norm, diffuse, specular);

	outColor.rgb *= material.hue;
	outColor.a = diffuse.a;
}

vec3 calculatePointLight(PointLight light, vec3 norm, vec4 materialDiffuse, vec4 materialSpecular)
{
	vec3 lightVec = light.position.xyz - position;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;


	//The light contributes to this surface

	//Calculate the attenuation(falloff)
	float attenuation = 1.0 / (light.constantAttenuation + (light.linearAttenuation * dist) + (light.quadraticAttenuation * dist * dist));

	vec3 ambient = attenuation * light.ambient;

	//Calculate the diffuse contribution
	float NdotL = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * NdotL * attenuation * materialDiffuse.rgb;

	vec3 reflectDir = reflect(-lightDir, norm);
	float VdotR = max(dot(normalize(-position), reflectDir), 0.0);

	vec3 specular = light.specular * pow(VdotR, material.specularExponent) * attenuation * materialSpecular.rgb;

	return ambient + diffuse + specular;
}
