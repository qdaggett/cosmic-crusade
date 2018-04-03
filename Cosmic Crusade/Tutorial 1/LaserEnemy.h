#pragma once
#include "Enemy.h"

class LaserEnemy : public Enemy
{
public:
	LaserEnemy();
	~LaserEnemy();

	void Intialize();
	void Intialize(bool move);

	void update(std::vector<Player*>, std::vector<Projectile*>*);
	void shoot(std::vector<Player*>, std::vector<Projectile*>*);

	GameObject laserObject;
	ParticleEmitterSoA* laserEmitter;

	void destroy();
private:
	bool moveY;

	float speed = 0.f;

	enum SpawnQuad{None, TopLeft, TopRight, BottomLeft, BottomRight};
	SpawnQuad quad;
};