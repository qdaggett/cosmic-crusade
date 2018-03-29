#pragma once
#include <GL/glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "Game.h"

const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND;

Game* game;

void DisplayCallbackFunction()
{
	game->draw();
}

void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	game->keyboardDown(key, x, y);
}

void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	game->keyboardUp(key, x, y);
}

void TimerCallbackFunction(int value)
{
	static int elapsedTimeAtLastTick = 0;
	int totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	game->deltaTime = float(totalElapsedTime - elapsedTimeAtLastTick);
	game->deltaTime /= 1000.0f;
	elapsedTimeAtLastTick = totalElapsedTime;

	game->update();

	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0);
}

void MouseClickCalbackFunction(int button, int state, int x, int y)
{
	game->mouseClicked(button, state, x, y);
}


void MouseMotionCallbackFunction(int x, int y)
{
	game->mouseMoved(x, y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitWindowSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Cosmic Crusade");
	glutFullScreen();

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW could not be initialized." << std::endl;
		system("pause");
		return 0;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	//setup callback functions
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutMouseFunc(MouseClickCalbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);
	
		//initialize game
	game = new Game();
	game->initializeGame();

	glutMainLoop();

	return 0;
}