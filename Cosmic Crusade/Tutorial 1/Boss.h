#pragma once
#include "Enemy.h"

class Boss : public Enemy
{
public:
	Boss();
	~Boss();
	void update(std::vector<Player*>, std::vector<Projectile*>*);
	void updateTime();
	void shoot(std::vector<Player*>, std::vector<Projectile*>*);
	void shootBasic(std::vector<Player*>, std::vector<Projectile*>*);
	void shootCircle(std::vector<Player*>, std::vector<Projectile*>*);
	void Intialize() {};
																		
	float lerpTime = 0.0f;

private:
	glm::vec2 lerpTarget;

	float basicDelay = 0.75f;
	float spinDelay = 0.3f;
	float circleDelay = 1.0f;

	float spinTime = 0.0f;
	float 	circleTime = 0.0f;
	float shootAng = 0.0f;

};