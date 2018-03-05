#pragma once
#include "Enemy.h"

class CircleEnemy : public Enemy
{
public:
	CircleEnemy();
	~CircleEnemy();

	void update(std::vector<Player*>, std::vector<Projectile*>*);
	void shoot(std::vector<Player*>, std::vector<Projectile*>*);
	void Intialize();
	void lerp(float x, float y, float dt);

	glm::vec2 target;

};