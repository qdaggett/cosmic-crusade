#pragma once
#include "Enemy.h"

class OrbitEnemy : public Enemy
{
public:
	OrbitEnemy();
	~OrbitEnemy();

	void update(std::vector<Player*>, std::vector<Projectile*>*);
	void shoot(std::vector<Player*>, std::vector<Projectile*>*);

	float distanceToPlayer = 8.f;
	bool orbit = false;
	bool boundary;
};