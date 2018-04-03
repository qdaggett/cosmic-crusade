#include "Background.h"
#include <iostream>
Background::Background()
{
}

Background::~Background()
{

}

void Background::Initialize()
{
	if (!back1.mesh.loadFromFile("meshes/plane.obj"))
	{
		std::cout << "background model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!back2.mesh.loadFromFile("meshes/plane.obj"))
	{
		std::cout << "background model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	loading.loadTexture(Diffuse, "Textures/Loading.png");

	game.loadTexture(Diffuse, "Textures/background.jpg");

	end.loadTexture(Diffuse, "Textures/End_Screen.jpg");

	title.loadTexture(Diffuse, "Textures/Title.png");
	win.loadTexture(Diffuse, "Textures/WinScreen.png");

	board.loadTexture(TextureType::Diffuse, "Textures/leaderboard.png");

	back1.scale = glm::scale(back1.scale, glm::vec3(scaleY, 1, scaleY));
	back1.rotate = glm::rotate(back1.rotate, angle, glm::vec3(1, 0, 0));
	back1.setLocation(0, -7, -3);
	back1.ogLoc = back1.location;

	back2.rotate = glm::rotate(back2.rotate, angle, glm::vec3(1, 0, 0));
	back2.scale = glm::scale(back2.scale, glm::vec3(scaleY, 1, scaleY));
	back2.setLocation(0, 68, -3);
	back2.ogLoc = back2.location;

	back1.mat = title;
	back2.mat = title;

	//std::cout << "Back moved up: " << back1.location.y << ", " << back2.location.y << std::endl;
}

void Background::mainMenu()
{
	back1.mat = title;
	back2.mat = title;

	back1.setLocation(0, -7, 0);
	back1.ogLoc = back1.location;

	back2.setLocation(0, 68, 0);
	back2.ogLoc = back2.location;
}

void Background::gameOver()
{

	back1.mat = end;
	back2.mat = end;

	if(back1.location != glm::vec2(0, 0))
		back1.setLocation(0, -7, 0);

	if(back2.location != glm::vec2(0, 75))
		back2.setLocation(0, 68, 0);

}

void Background::winScreen()
{
	back1.mat = win;
	back2.mat = win;

	if (back1.location != glm::vec2(0, 0))
		back1.setLocation(0, -7, 0);

	if (back2.location != glm::vec2(0, 75))
		back2.setLocation(0, 68, 0);
}

void Background::restart()
{
	back1.mat = game;
	back2.mat = game;
}

void Background::update()
{
	updateTimer->tick();
	localTime += updateTimer->getElapsedTimeS();

	if (localTime >= 1.0f)
	{
		localTime = 0.0f;
		back1.ogLoc = back1.location;
		back2.ogLoc = back2.location;
	}

	back1.lerp(0, back1.ogLoc.y - 5.0f, localTime);
	back2.lerp(0, back2.ogLoc.y - 5.0f, localTime);

	if (back1.location.y <= -68)
	{
		back1.setLocation(0, 68, 0);
	}
	
	if (back2.location.y <= -68)
	{
		back2.setLocation(0, 68, 0);
	}
}

void Background::leaderBoard()
{
	back1.mat = board;
	back2.mat = board;

	back1.setLocation(0, 0);
}

void Background::draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, Light pointLights)
{
	back1.draw(shader, cameraTransform, cameraProjection, pointLights);
	back2.draw(shader, cameraTransform, cameraProjection, pointLights);
}