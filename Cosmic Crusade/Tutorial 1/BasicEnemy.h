#pragma once
#include "Enemy.h"
#include <iostream>

class BasicEnemy : public Enemy
{
public:
	BasicEnemy();
	~BasicEnemy();

	void update(std::vector<Player*>, std::vector<Projectile*>*);
	void shoot(std::vector<Player*>, std::vector<Projectile*>*);
	

};