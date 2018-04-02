#include <iostream>
#include <math.h>
#include "Player.h"

float Player::progress = 0.0f;
float const Player::transformMax = 15.0f;

// 15/3/2018
Player::Player()
{
	updateTimer = new Timer();
	updateTimer->tick();
	collider = new Collider(Collider::BOX, glm::vec3(1.5f, 1.5f, 0));
	//yellowBar.scale = glm::scale(yellowBar.ogScale, glm::vec3(.3f, .1f, .3f));
}

Player::~Player()
{
	//Delete pointers
	for (int i = 0; i < projectiles.size(); i++)
	{
		delete projectiles[i];
	}
}

void Player::update(Player* otherPlayer)
{
	updateTimer->tick();
	collider->ColliderUpdate(glm::vec3(location, 0));
	xin(otherPlayer);

	if ((isTransformed && otherPlayer->isTransformed) && progress >= 0.0f)
	{
		progress -= updateTimer->getElapsedTimeS() * 0.5f;
		progress -= updateTimer->getElapsedTimeS();
		blackBar.scale = glm::scale(blackBar.ogScale, glm::vec3((transformMax - progress) / transformMax, .3f, .3f));
		blackBar.move(0.0f, 0.0f);
	}

	if (progress <= 0.0f)
	{
		progress = 0.0f;
		otherPlayer->progress = 0.0f;
		isTransformed = false;
		otherPlayer->isTransformed = false;
	}

	turret.setLocation(location.x, location.y);
	blackBar.setLocation(location.x, location.y - 3.0f);
	yellowBar.setLocation(location.x, location.y - 3.0f);
}

void Player::updateProjectiles(std::vector<Enemy*>* enemies, Player* otherPlayer, std::vector<ParticleEmitterSoA*>* emitters)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		//projectiles[i]->move(projectiles[i]->getVelocity().x, projectiles[i]->getVelocity().y);

		projectiles[i]->update();

		if (projectiles[i]->isOffscreen())
		{
			projectiles.erase(projectiles.begin() + i);
			break;
		}

		std::vector<Enemy*> derefEnemies = *enemies;
		//Iterate through each enemy, check if current projectile is intersecting with it
		for (int j = 0; j < enemies->size(); j++)
		{
			GameObject temp;
			temp.location = derefEnemies[j]->location;
			temp.radius = derefEnemies[j]->radius;
			temp.collider = derefEnemies[j]->collider;

			if (projectiles[i]->collide(temp))
			{
				hasHit = true;
				score += 10;
				hits++;
				derefEnemies[j]->hitPoints--;
				derefEnemies[j]->gotDamaged = true;
				tempEnemy = derefEnemies[j]->location;

				if (progress < transformMax)
				{
					progress++;
					otherPlayer->progress++;

					blackBar.scale = glm::scale(blackBar.ogScale, glm::vec3(.3f - (.3f * (progress / transformMax)), .3f, .3f));
					blackBar.move(0, 0);

					otherPlayer->blackBar.scale = glm::scale(otherPlayer->blackBar.ogScale, glm::vec3(.3f - (.3f * (progress / transformMax)), .3f, .3f));
					otherPlayer->blackBar.move(0, 0);

					//Erase projectile, Erase enemy
					deleteProjectile(i);

					ParticleEmitterSoA* exp = new ParticleEmitterSoA();

					exp->explosionInit(glm::vec3(-temp.location.x, temp.location.y, 1.0f));
					exp->texture = derefEnemies[j]->deathColour.diffuse;
					
					emitters->push_back(exp);

					enemies->erase(enemies->begin() + j);
				}

				else
				{
					//Erase projectile, Erase enemy
					deleteProjectile(i);

					ParticleEmitterSoA* exp = new ParticleEmitterSoA();

					exp->explosionInit(glm::vec3(-temp.location.x, temp.location.y, 1.0f));
					exp->texture = derefEnemies[j]->deathColour.diffuse;

					emitters->push_back(exp);

					enemies->erase(enemies->begin() + j);
				}
				break;
			}
		}
	}
}

//Getting input from the controller
void Player::xin(Player* otherPlayer)
{
	//Used for shooting delay
	localTime += updateTimer->getElapsedTimeS();
	
	shotgunTime += updateTimer->getElapsedTimeS();

	speedDelayTime += updateTimer->getElapsedTimeS();

	//poll controllers
	controller.DownloadPackets(2);
	controller.GetSticks(playerNum, lStick, rStick);

	//Move the controller in the x axis based on the left stick's x axis, and they y likewise
	if (playerNum == 1 && (isTransformed && otherPlayer->getTransform()))
	{
		//if (!isTransformed || !otherPlayer->getTransform())
			//move(lStick.xAxis * 0.25f, lStick.yAxis * 0.25f);

		//else
		{
			setLocation(otherPlayer->location.x, otherPlayer->location.y);

			glm::vec2 normalDir = glm::vec2(lStick.xAxis, lStick.yAxis);
			float length = sqrt((normalDir.x * normalDir.x) + (normalDir.y * normalDir.y));
			normalDir.x /= length;
			normalDir.y /= length;

			shield.setLocation(location.x, location.y);

			bool tilted = std::abs(lStick.xAxis) > 0.25f || std::abs(lStick.yAxis) > 0.25f ? true : false;

			if (tilted)
			{
				shield.collider->boxCollider.size = glm::vec3(1, 0.5f, 0);
				if (normalDir.x <= 0.0f)
				{
					shield.rotate = glm::rotate(shield.ogRotate, acos(normalDir.y), glm::vec3(0.0f, 0.0f, 1.0f));
				}

				else
				{
					shield.rotate = glm::rotate(shield.ogRotate, -acos(normalDir.y), glm::vec3(0.0f, 0.0f, 1.0f));
				}

				shield.move(normalDir.x * 3.0f, normalDir.y * 3.0f);
			}
			else
				shield.collider->boxCollider.size = glm::vec3();


		}
	}

	if (speedDelayTime > speedDelay && otherPlayer->getSpeedUp() < 100)
	{
		speedDelayTime = 0;
		otherPlayer->addSpeedUp(1);
	}

	if (controller.GetButton(playerNum, XBox::RB) && (otherPlayer->getSpeedUp() > 0))
	{
		move(lStick.xAxis * 0.55f, lStick.yAxis * 0.55f);
		otherPlayer->addSpeedUp(-1);

	}
	else
	{
		move(lStick.xAxis * 0.35f, lStick.yAxis * 0.35f);
	}

	rotate = glm::rotate(ogRotate, lStick.xAxis * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

	//Checking if the right stick is tilted more than a certain amount. tilted will be true if the right stick is being tilted.
	bool tilted = std::abs(rStick.xAxis) > 0.25f || std::abs(rStick.yAxis) > 0.25f ? true : false;

	//Shoot after they delay, and reset delay
	if (tilted)
	{
		if (localTime > delay)
		{
			shoot();
			localTime = 0;
		}

		glm::vec2 normalVel = glm::vec2(rStick.xAxis, rStick.yAxis);
		float length = sqrt((normalVel.x * normalVel.x) + (normalVel.y * normalVel.y));
		normalVel.x /= length;
		normalVel.y /= length;

		//Essentially finding and rotating by the dot product with the unit y vector (0, 1)... It will just return normalVel.y
		//Rotation value will always be between 0 and 180, so we need to just check which x direction the projectile is going to fix rotation
		if (normalVel.x <= 0.0f)
		{
			reticle.rotate = glm::rotate(reticle.ogRotate, acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
			turret.rotate = glm::rotate(turret.ogRotate, acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		else
		{
			reticle.rotate = glm::rotate(reticle.ogRotate, -acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
			turret.rotate = glm::rotate(turret.ogRotate, -acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		reticle.setLocation(location.x + (3.0f * normalVel.x), location.y + (3.0f * normalVel.y));
	}

	if (controller.GetButton(playerNum, XBox::LB) && (shotgunTime > shotgunDelay))
	{
		if (otherPlayer->getAmmo() > 0)
		{
			shotgunTime = 0;
			shootShotgun();
			otherPlayer->addAmmo(-1);
		}

	}

	if (controller.GetButton(playerNum, XBox::LB))
	{
		std::cout << playerNum << ", " << progress << std::endl;
	}

	if (controller.GetButton(playerNum, XBox::LB) && (progress == transformMax))
	{
		isTransformed = true;
		std::cout << isTransformed << std::endl;
	}
}

//Getters
XBox::Stick Player::getLStick()
{
	return lStick;
}

XBox::Stick Player::getRStick()
{
	return rStick;
}

std::vector<Projectile*> Player::getProjectiles()
{
	return projectiles;
}

void Player::setNum(int num)
{
	playerNum = num;
}

void Player::shoot()
{
	hasShot = true;
	//Projectile pointer to be pushed back into the vector of projectiles
	Projectile* temp = new Projectile();

	//Set the mesh of the new projectile
	temp->mesh = projectile.mesh;
	temp->mat = projectile.mat;

	temp->collider = new Collider(Collider::BOX, glm::vec3(0.5f, 1, 0));

	//Make the new projectile's starting location equal to the player's 
	//temp->transform = transform;
	temp->location = glm::vec2(0.0f, 0.0f);
	temp->move(location.x, location.y);


	//Normalize the projectile's velocity vector so that projectiles will fire at the same speed regardless of the amount of tilt amount
	glm::vec2 normalVel = glm::vec2(rStick.xAxis, rStick.yAxis);
	float length = sqrt((normalVel.x * normalVel.x) + (normalVel.y * normalVel.y));
	normalVel.x /= length;
	normalVel.y /= length;

	//Essentially finding and rotating by the dot product with the unit y vector (0, 1)... It will just return normalVel.y
	//Rotation value will always be between 0 and 180, so we need to just check which x direction the projectile is going to fix rotation
	if (normalVel.x <= 0.0f)
	{
		temp->rotate = glm::rotate(temp->rotate, acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	else
	{
		temp->rotate = glm::rotate(temp->rotate, -acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	//Assign velovity
	temp->velocity = glm::vec2(0.5 * normalVel.x, 0.5 * normalVel.y);

	totalShots++;
	//std::cout << getAccuracy() << std::endl;

	projectiles.push_back(temp);
}

void Player::shootShotgun()
{
	hasShotShotgun = true;
	float increment = 0.27f / 4.0f;

	for (float i = 0; i < 12; i++)
	{
		Projectile* temp = new Projectile();
		temp->mesh = projectile.mesh;
		temp->mat = projectile.mat;
		temp->collider = new Collider(Collider::SPHERE, 0.5f);

		temp->location = glm::vec2(0.0f, 0.0f);
		temp->move(location.x, location.y);

		glm::vec2 dir = glm::vec2(cos(increment * i - 5.0), sin(increment * i - 5.0));
		float length = sqrt((dir.x * dir.x) + (dir.y * dir.y));
		dir.x /= length;
		dir.y /= length;

		if (dir.x <= 0.0f)
		{
			temp->rotate = glm::rotate(temp->rotate, acos(dir.y), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		else
		{
			temp->rotate = glm::rotate(temp->rotate, -acos(dir.y), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		temp->velocity = glm::vec2(0.4 * dir.x, 0.4 * dir.y);

		projectiles.push_back(temp);

	}

}

void Player::deleteProjectile(int index)
{
	//delete projectiles[index];
	projectiles.erase(projectiles.begin() + index);
}

bool Player::isAlive()
{
	if (playerState == state::alive)
		return true;

	return false;
}

bool Player::getTilted()
{
	return std::abs(rStick.xAxis) > 0.25f || std::abs(rStick.yAxis) > 0.25f ? true : false;
}

int Player::getAmmo()
{
	return ammoCount;

}

void Player::addAmmo(int ammo)
{
	ammoCount += ammo;

}

int Player::getSpeedUp()
{
	return speedUp;
}

void Player::addSpeedUp(int fuel)
{
	speedUp += fuel;
}
