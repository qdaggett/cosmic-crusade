#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class GameObject
{
public:
	//GameObject();

	//Constructor with defalt values
	GameObject(glm::vec2 loc = glm::vec2(0.0f, 0.0f), float rad = 0.5f);
	~GameObject();

	void move(float x, float y);
	void move(float x, float y, float z);
	void setLocation(float x, float y);
	void setLocation(float x, float y, float z);

	//Will hold the object's location. vec2 because z axis doesn't matter
	glm::vec2 location = glm::vec2(0.0f, 0.0f);

	bool collide(GameObject other);
	void draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, std::vector<Light> pointLights);
	void draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, Light pointLight);
	void loadTexture(TextureType type, const std::string &texFile);

	//For collision sphere
	float radius = 0.5f;

	//Transformation matrix
	glm::mat4 transform;

	glm::mat4 translate;
	glm::mat4 rotate;
	glm::mat4 ogRotate;
	glm::mat4 scale;
	glm::mat4 ogScale;

	Mesh mesh;
	Material mat;

private:
	
};