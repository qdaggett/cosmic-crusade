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
#include "ParticleEmitterSoA.h"
//#include "Camera.h"
#include "PowerUp.h"
#include "Ammo.h"
#include "Fuel.h"
#include "Time.h"

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
	//void initializeLevel();

	void doBrightPass();
	void doBlurPass();

	float deltaTime;
	glm::vec3 cameraTranslation;

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
	Ammo ammoPowerUp;
	Fuel fuelPowerUp;
	Time timePowerUp;

	//Shaders
	ShaderProgram phong, textShader, unlitShader, brightPass, blurShader, bloomShader, particleShader, orComposite;

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
	enum gameStates { main, gameOver, title, monologue, win };
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
	FrameBufferObject def, bright, blur_a, blur_b, lowRes, toBloom, text_fbo, background_fbo;
	std::vector<FrameBufferObject*> fbos;
	ParticleEmitterSoA emitter;
	std::vector<ParticleEmitterSoA*> emitters;
	EnemyManager enemyManager;
};