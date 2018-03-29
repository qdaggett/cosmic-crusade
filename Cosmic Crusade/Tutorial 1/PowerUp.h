#pragma once
#include "GameObject.h"
#include "Player.h"
#include "glm\glm.hpp"

// Quinn Daggett - 100618734

class PowerUp : public GameObject
{
public:

	// Constructor and Destructor
	PowerUp();
	~PowerUp();

	// Functions for main
	virtual void updatePowerUp(std::vector<Player*>, float time) = 0;
	virtual void drawPowerUp(ShaderProgram &shader, glm::mat4 camera, glm::mat4 projection, std::vector<Light> lights) = 0;
	virtual void initializePowerUp() = 0;
	virtual void spawnPowerUp(glm::vec2 position) = 0;

	float getLifetime();

	Timer* updateTimer = nullptr;
	float localTime = 0.0f;
	float rotationTick;
	float powerupCoolDown = 5.0f;
	bool powerupIsActive = false;

private:
	float lifetime = 10.0f;

};