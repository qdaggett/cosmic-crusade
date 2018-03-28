#include "FullScreenQuad.h"

FullScreenQuad::FullScreenQuad()
{

}

FullScreenQuad::~FullScreenQuad()
{

}

void FullScreenQuad::create()
{

	vertices.reserve(6); // performance tip, explain how vectors work
	textureCoordinates.reserve(6);

	vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	vertices.push_back(glm::vec3(-1.0, -1.0, 0.0f));

	vertices.push_back(glm::vec3(1.0, 1.0, 0.0f));
	vertices.push_back(glm::vec3(-1.0, -1.0, 0.0f));
	vertices.push_back(glm::vec3(1.0, -1.0, 0.0f));

	textureCoordinates.push_back(glm::vec3(glm::vec2(1.0f, 1.0f), 0.0f));
	textureCoordinates.push_back(glm::vec3(glm::vec2(0.0f, 1.0f), 0.0f));
	textureCoordinates.push_back(glm::vec3(glm::vec2(0.0f, 0.0f), 0.0f));

	textureCoordinates.push_back(glm::vec3(glm::vec2(1.0f, 1.0f), 0.0f));
	textureCoordinates.push_back(glm::vec3(glm::vec2(0.0f, 0.0f), 0.0f));
	textureCoordinates.push_back(glm::vec3(glm::vec2(1.0f, 0.0f), 0.0f));

	colours = std::vector<glm::vec4>(6, glm::vec4(0, 0, 0, 1));

	unsigned int numTris = (unsigned int)(vertices.size()) / 3; // todo: handle non-triangulated meshes
																// Setup VBO
																// Set up position (vertex) attribute
	if (vertices.size() > 0)
	{
		AttributeDescriptor positionAttrib;
		positionAttrib.attributeLocation = AttributeLocations::VERTEX;
		positionAttrib.attributeName = "vertex";
		positionAttrib.data = &vertices[0];
		positionAttrib.elementSize = sizeof(float);
		positionAttrib.elementType = GL_FLOAT;
		positionAttrib.numElements = numTris * 3 * 3; // (num triangles * three vertices per triangle * three floats per vertex)
		positionAttrib.numElementsPerAttrib = 3;
		vbo.addAttributeArray(positionAttrib);
	}

	// Set up UV attribute
	if (textureCoordinates.size() > 0)
	{
		AttributeDescriptor uvAttrib;
		uvAttrib.attributeLocation = AttributeLocations::TEX_COORD;
		uvAttrib.attributeName = "uv";
		uvAttrib.data = &textureCoordinates[0];
		uvAttrib.elementSize = sizeof(float);
		uvAttrib.elementType = GL_FLOAT;
		uvAttrib.numElements = numTris * 3 * 2;
		uvAttrib.numElementsPerAttrib = 2;
		vbo.addAttributeArray(uvAttrib);
	}

	// Set up normal attribute
	if (normals.size() > 0)
	{
		AttributeDescriptor normalAttrib;
		normalAttrib.attributeLocation = AttributeLocations::NORMAL;
		normalAttrib.attributeName = "normal";
		normalAttrib.data = &normals[0];
		normalAttrib.elementSize = sizeof(float);
		normalAttrib.elementType = GL_FLOAT;
		normalAttrib.numElements = numTris * 3 * 3;
		normalAttrib.numElementsPerAttrib = 3;
		vbo.addAttributeArray(normalAttrib);
	}

	// set up other attributes...

	vbo.createVBO(GL_STATIC_DRAW);
}

void FullScreenQuad::draw(ShaderProgram &shader)
{
	shader.bind();
	shader.sendUniformMat4("u_mvp", glm::value_ptr(glm::mat4()), false);

	glActiveTexture(GL_TEXTURE0);
	vbo.draw();
	shader.unbind();
}