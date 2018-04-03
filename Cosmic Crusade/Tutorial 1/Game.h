#pragma once
#include "FrameBufferObject.h"
#include "ShaderProgram.h"
#include "Player.h"
#include "BasicEnemy.h"
#include "CircleEnemy.h"
#include "OrbitEnemy.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Background.h"
#include <WinUser.h>
#include <vector>
#include "Font.h"
#include "Foreground.h"
#include "SoundEngine.h"
#include "EnemyManager.h"
#include "SoundEffect.h"
#include "FullScreenQuad.h"
#include <fstream>

#define FRAMES_PER_SECOND 60

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	//Input Callback Functions
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	void emptyGame();
	void initializeLevel();

	void LoadScores();
	void SetScores(); 

	void SaveScores();

	void doBrightPass();
	void doBlurPass();

	// FMOD Sound handler
	SoundEffect gameSounds;

	//Data Members
	Timer* updateTimer = nullptr;
	Player player;
	Player player2;
	std::vector<Player*> players;

	std::vector<Light> pointLights;

	Material red;
	Material blue;
	Material yellow;
	Material green;
	Material purple;
	Material bluePlayer;
	Material yellowPlayer;

	GameObject basicPlayer;
	GameObject combinedPlayer;

	//Shaders
	ShaderProgram phong, textShader, unlitShader, brightPass, blurShader, bloomShader, rimShader;

	ShaderProgram phongColorSides;
	glm::mat4 cameraTransform;
	glm::mat4 cameraProjection;
	glm::mat4 cameraOrtho;
	glm::mat4 lightSpinner;

private:
	unsigned int currentEnemy = 0;

	// Pause functionality
	bool paused;
	float pauseDelay = 0.25f;
	float pauseTime = 0.0f;

	bool shouldLightsSpin = false;
	enum gameStates { main, gameOver, title, monologue, score };
	gameStates state = title;
	float delay;

	bool debugSelect;

	bool empty = false;

	Light backgroundLight;
	Background background;

	GameObject playButton, quitButton;
	enum selectedButton { _play, _quit, none };
	selectedButton selected = _play;
	Material play, play_un, quit, quit_un;
	FullScreenQuad fullscreenQuad;

	Font text;

	Foreground foreground;
	FrameBufferObject def, bright, blur_a, blur_b, lowRes, toBloom, rimBuffer;

	EnemyManager enemyManager;

	glm::vec3 bossCameraPosition = glm::vec3(0, 0, -30);

	float cameraFloat = 0;


	//Variables used to save players scores and using .txt file
	fstream file;

	std::vector<std::string> scoreNames;
	std::vector<int> scoreValues;

	bool scoreSet;

	char name1[4];
	char name2[4];
	int itr1 = 0, itr2 = 0;

	bool setScores, playerTwoType;		//saveScores just checks when

	int player1Spot = 0, player2Spot = 0;


};