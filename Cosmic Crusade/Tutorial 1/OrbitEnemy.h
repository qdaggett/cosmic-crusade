#pragma once
#include "Enemy.h"

class OrbitEnemy : public Enemy
{
public:
	OrbitEnemy();
	~OrbitEnemy();

	void update(std::vector<Player*>, std::vector<Projectile*>*);
	void shoot(std::vector<Player*>, std::vector<Projectile*>*);
	void Intialize();

	float distanceToPlayer = 8.f;
	bool orbit = false;
	bool boundary;
	glm::vec2 targetPosition = glm::vec2();
};