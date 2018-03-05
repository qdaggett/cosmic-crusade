#pragma once
#include "VertexBufferObject.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

class FullScreenQuad
{
public:
	FullScreenQuad();
	~FullScreenQuad();

	void create();
	void draw(ShaderProgram &shader);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec4> colours;

	VertexBufferObject vbo;
};