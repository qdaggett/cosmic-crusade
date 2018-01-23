#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	friend class Player;
	friend class Enemy;
	Projectile();
	//Initialize the projectile with an x and y velocity
	Projectile(float x, float y);
	~Projectile();

	bool isOffscreen();

	glm::vec2 getVelocity();
	glm::vec2 velocity;

private:
	//Projectiles will be given an x and y velocity based on the direction the right controller stick is tilted. 

};