#pragma once
#include <glm\glm.hpp>
#include "Texture.h"

class Material
{
public:
	Material();
	Material(std::string diffusePath, std::string specularPath, float specular = 50.0f, glm::vec3 hue = glm::vec3(1.0f, 1.0f, 1.0f));
	~Material();

	void loadTexture(TextureType type, std::string filePath);

	Texture diffuse, specular;
	glm::vec3 hue = glm::vec3(1.0f, 1.0f, 1.0f);
	float specularExponent = 50.0f;
};