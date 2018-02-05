#pragma once
#include <vector>
#include "Timer.h"
#include "Projectile.h"
#include "Enemy.h"
#include "SoundEngine.h"


class Player : public GameObject
{
public:
	friend class Game;
	Player();
	~Player();

	//Getting input from the controller
	void xin(Player* otherPlayer);
	void shoot();
	bool hasShot;
	bool hasHit;
	void update(std::vector<Enemy*>* enemies, Player* otherPlayer);

	XBox::Stick getLStick(); 
	XBox::Stick getRStick();

	std::vector<Projectile*> getProjectiles();
	void setNum(int num);
	Projectile projectile;
	void deleteProjectile(int index);
	Timer* updateTimer = nullptr;

	bool getTransform() { return isTransformed; }
	enum state { alive, dead };
	bool isAlive();
	void setDead() { playerState = dead; }
	void setAlive() { playerState = alive; }

	float progress = 0;
	float transformMax = 15;

	float getAccuracy()
	{
		if (totalShots <= 0)
			return 0;

		return trunc((hits / totalShots) * 100);
	}

	GameObject blackBar;
	GameObject yellowBar;

	GameObject shield;

	unsigned int score = 0;

private: 

	state playerState = alive;
	XBox::XBoxInput controller;	
	XBox::Stick lStick;
	XBox::Stick rStick;
	int playerNum;
	unsigned int numLives = 3;
	float delay = 0.25f;
	float localTime = 0.0f;
	float spawnTime = 0.0f;
	bool isTransformed = false;
	unsigned int hits = 0;
	unsigned int totalShots = 0;


	Material baseMat;
	Material transformMat;

	//Vector containing all the projectiles the player has fired. Stored in player for collision purposes. 
	std::vector<Projectile*> projectiles;
};