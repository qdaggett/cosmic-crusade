#pragma once
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

	// FMOD data members
	SoundEngine se;
	FMOD::Sound *failsound;
	FMOD::Channel *channel = 0;
	FMOD_VECTOR soundPos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };
	FMOD_RESULT result;

	//Data Members
	Timer* updateTimer = nullptr;
	Player player;
	Player player2;
	std::vector<Player*> players;

	//Vector of enemies, because we will be drawing some arbitrary amount
	BasicEnemy basicEnemy;
	CircleEnemy circleEnemy;
	OrbitEnemy orbitEnemy;
	std::vector<Enemy*> enemies;
	std::vector<Projectile*> enemyProjectiles;
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
	ShaderProgram phong;
	ShaderProgram phongColorSides;
	ShaderProgram textShader;

	glm::mat4 cameraTransform;
	glm::mat4 cameraProjection;
	glm::mat4 cameraOrtho;
	glm::mat4 lightSpinner;

private:

	enum enemyType { basic, circle, orbit };
	unsigned int currentEnemy = 0;

	struct enemyNode
	{
		enemyNode(float time, glm::vec2 loc, enemyType nmy) : spawnTime(time), location(loc), type(nmy){}
		float spawnTime;
		glm::vec2 location;
		enemyType type;
		
	};

	std::vector<enemyNode*> level1;


	bool shouldLightsSpin = false;
	void updateEnemyProjectiles();
	enum gameStates{main, gameOver, title};
	gameStates state = title;
	float delay;

	bool debugSelect;

	bool empty = false;

	Light backgroundLight;
	Background background;

	GameObject playButton, quitButton;
	enum selectedButton{ _play, _quit, none };
	selectedButton selected = _play;
	Material play, play_un, quit, quit_un;

	Font text;

	Foreground foreground;
};