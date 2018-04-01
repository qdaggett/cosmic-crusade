#pragma once

#include "PowerUp.h"

class Ammo : public PowerUp
{
public:
	Ammo();
	~Ammo();

	void updatePowerUp(std::vector<Player*>, float time);
	void drawPowerUp(ShaderProgram &shader, glm::mat4 camera, glm::mat4 projection, std::vector<Light> lights);
	void spawnPowerUp(glm::vec2 position);
	void initializePowerUp();

	bool collected = false;

};