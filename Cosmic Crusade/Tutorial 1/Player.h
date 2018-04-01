#pragma once
#include <vector>
#include "Timer.h"
#include "Projectile.h"
#include "Enemy.h"
#include "SoundEngine.h"
#include "SoundEffect.h"
#include "ParticleEmitterSoA.h"

// 15/3/2018

class Player : public GameObject
{

public:
	friend class Game;
	Player();
	~Player();

	//Getting input from the controller
	void xin(Player* otherPlayer);
	void shoot();

	// Shotgun functions
	void update(Player* otherPlayer);
	void updateProjectiles(std::vector<Enemy*>* enemies, Player* otherPlayer, std::vector<ParticleEmitterSoA*>* emitters);
	void shootShotgun();
	int getAmmo();
	void addAmmo(int ammo);
	
	// Speedup functions
	int getSpeedUp();
	void addSpeedUp(int fuel);


	void UpdateProjectiles(std::vector<Enemy*>* enemies, Player* otherPlayer);

	bool hasShot;
	bool hasShotShotgun;
	bool hasHit;
	

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
	void setAlive() { playerState = alive; invulnerable = true; }

	bool getTilted();
	bool isInvulnerable()
	{
		return invulnerable;
	}
	

	state getState()
	{
		return playerState;
	}

	float getAccuracy()
	{
		if (totalShots <= 0)
			return 0;

		return trunc((hits / totalShots) * 100);
	}

	GameObject blackBar, yellowBar, shield, turret, reticle;

	unsigned int score = 0;

	unsigned int numLives = 3;

	unsigned int hits = 0;
	unsigned int totalShots = 0;


	//Static stuff
	static float progress;
	static const float transformMax;

private: 

	state playerState = alive;
	XBox::XBoxInput controller;	
	XBox::Stick lStick;
	XBox::Stick rStick;
	int playerNum;

	float delay = 0.25f;
	float localTime = 0.0f;
	float spawnTime = 0.0f;
	bool isTransformed = false;

	bool invulnerable;
	float invulnerableTime = 3.75f;
	float invulnerableTimer;

	float dt;

	// Shotgun data members
	float shotgunDelay = 1.50f;
	float shotgunTime = 0.0f;
	int ammoCount = 3;

	float speedDelay = 0.25f;
	float speedDelayTime = 0.0f;
	int speedUp = 100;


	Material baseMat;
	Material transformMat;

	//Vector containing all the projectiles the player has fired. Stored in player for collision purposes. 
	std::vector<Projectile*> projectiles;
};
