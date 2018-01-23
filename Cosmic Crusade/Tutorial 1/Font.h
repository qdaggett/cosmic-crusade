#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GL\glut.h>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "ShaderProgram.h"

struct  Character
{
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

class Font {

private:
	FT_Library ft;
	FT_Face face;
	std::map<GLchar, Character> Characters;
	//ShaderProgram shader;

	GLuint VAO, VBO;
public:
	Font();
	~Font();
	void Intialize(std::string font);
	void RenderText(ShaderProgram &s, glm::mat4 projection, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};