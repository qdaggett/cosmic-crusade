#include "Material.h"
#include <iostream>

Material::Material()
{

}

Material::Material(std::string diffusePath, std::string specularPath, float specular, glm::vec3 hue) : specularExponent(specular), hue(hue)
{
	loadTexture(TextureType::Diffuse, diffusePath);
	loadTexture(TextureType::Specular, specularPath);
}

Material::~Material()
{

}

void Material::loadTexture(TextureType type, std::string filePath)
{
	switch (type)
	{
	case(Diffuse):
		if (!diffuse.load(filePath))
		{
			system("pause");
			exit(0);
		}
		break;

	case(Specular):
		if (!specular.load(filePath))
		{
			system("pause");
			exit(0);
		}
		break;

	default:
		std::cout << "Error. Texture type not found : " << type << std::endl;
		system("pause");
		exit(0);
		break;
	}
}
