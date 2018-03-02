#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "controller.h"
#include "VertexBufferObject.h"

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

	void createFSQ();
	void draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection);

	//OpenGl buffers and objects
	GLuint vboVertices = 0;
	GLuint vboUVs = 0;
	GLuint vboNormals = 0;
	GLuint vao = 0;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec4> colours;

	VertexBufferObject vbo;

private:
	unsigned int numFaces = 0;
	unsigned int numVertices = 0;

};