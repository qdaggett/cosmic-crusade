#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "controller.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	//Load a mesh and sent it to OpenGL
	bool loadFromFile(const std::string &file);

	//Release data from OpenGl memory
	void unload();

	unsigned int getNumFaces();
	unsigned int getNumVertices();

	//OpenGl buffers and objects
	GLuint vboVertices = 0;
	GLuint vboUVs = 0;
	GLuint vboNormals = 0;
	GLuint vao = 0;

private:
	unsigned int numFaces = 0;
	unsigned int numVertices = 0;

};