#include <iostream>
#include <math.h>
#include "Player.h"

Player::Player()
{
	updateTimer = new Timer();
	updateTimer->tick();
}

Player::~Player()
{
	//Delete pointers
	for (int i = 0; i < projectiles.size(); i++)
	{
		delete projectiles[i];
	}
}

void Player::update(std::vector<Enemy*>* enemies, Player* otherPlayer)
{
	updateTimer->tick();
	xin(otherPlayer);
	std::vector<Enemy*> derefEnemies = *enemies;

	if ((isTransformed && otherPlayer->isTransformed) && progress >= 0.0f)
	{
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

	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->move(projectiles[i]->getVelocity().x, projectiles[i]->getVelocity().y);

		if (projectiles[i]->isOffscreen())
		{
			deleteProjectile(i);
			break;
		}

		//Iterate through each enemy, check if current projectile is intersecting with it
		for (int j = 0; j < enemies->size(); j++)
		{
			GameObject temp;
			temp.location = derefEnemies[j]->location;
			temp.radius = derefEnemies[j]->radius;

			if (projectiles[i]->collide(temp) && derefEnemies[j]->hitPoints != 0)
			{
				score += 10;
				hits++;
				derefEnemies[j]->hitPoints--;

				if(progress < transformMax)
				{
					progress++;
					otherPlayer->progress++;

					blackBar.scale = glm::scale(blackBar.ogScale, glm::vec3(.3f - (.3f * (progress / transformMax)), .3f, .3f));
					std::cout << 1.0f - (progress / transformMax) << std::endl;
					blackBar.move(0, 0);

					otherPlayer->blackBar.scale = glm::scale(otherPlayer->blackBar.ogScale, glm::vec3(.3f - (.3f * (progress / transformMax)), .3f, .3f));
					otherPlayer->blackBar.move(0, 0);

					//Erase projectile, Erase enemy
					deleteProjectile(i);

					//enemies->erase(enemies->begin() + j);
					break;
				}

				else
				{
					//Erase projectile, Erase enemy
					deleteProjectile(i);
				
					enemies->erase(enemies->begin() + j);
					break;
				}
			}
		}
	}
}

//Getting input from the controller
void Player::xin(Player* otherPlayer)
{
	//Used for shooting delay
	localTime += updateTimer->getElapsedTimeS();

	//poll controllers
	controller.DownloadPackets(2);
	controller.GetSticks(playerNum, lStick, rStick);

	//Move the controller in the x axis based on the left stick's x axis, and they y likewise
	if (playerNum == 1)
	{
		if (!isTransformed || !otherPlayer->getTransform())
			move(lStick.xAxis * 0.25f, lStick.yAxis * 0.25f);

		else
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
		}
	}

	else
		move(lStick.xAxis * 0.25f, lStick.yAxis * 0.25f);
	
	//Checking if the right stick is tilted more than a certain amount. tilted will be true if the right stick is being tilted.
	bool tilted = std::abs(rStick.xAxis) > 0.25f || std::abs(rStick.yAxis) > 0.25f ? true : false;

	//Shoot after they delay, and reset delay
	if (tilted && (localTime > delay))
	{
		localTime = 0;
		
		shoot();
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
	//Projectile pointer to be pushed back into the vector of projectiles
	Projectile* temp = new Projectile();

	//Set the mesh of the new projectile
	temp->mesh = projectile.mesh;
	temp->mat = projectile.mat;

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
	temp->velocity = glm::vec2(0.7 * normalVel.x, 0.7 * normalVel.y);

	totalShots++;
	//std::cout << getAccuracy() << std::endl;

	projectiles.push_back(temp);
}

void Player::deleteProjectile(int index)
{
	//delete projectiles[index];
	projectiles.erase(projectiles.begin() + index);
	std::cout << "Deleted prjectile : " << index << std::endl;
}

bool Player::isAlive()
{
	if (playerState == state::alive)
		return true;

	return false;
}
