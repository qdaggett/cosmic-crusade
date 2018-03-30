#pragma once
#include "GameObject.h"

class Background
{
public:
	Background();
	~Background();
	void Initialize();
	void gameOver();
	void restart();
	void mainMenu();

	void update();
	void draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, Light pointLights);

private:
	GameObject back1, back2;
	Material loading, title, game, end;
	const float angle = -55.0f;
	const float scaleY = 2.0f;
};