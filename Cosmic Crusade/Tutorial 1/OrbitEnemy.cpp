#include "OrbitEnemy.h"
#include <iostream>
OrbitEnemy::OrbitEnemy()
{
	glm::vec2 direction = glm::vec2() - location;

	velocity = glm::normalize(direction);
	delay = 2.0f;
}

OrbitEnemy::~OrbitEnemy()
{

}

void OrbitEnemy::update(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	//Used for shooting delay
	updateTimer->tick();
	localTime += updateTimer->getElapsedTimeS();

	glm::vec2 direction = glm::vec2() - location;

	velocity = glm::normalize(direction);

	//Shoot after they delay, and reset delay
	if (localTime > delay)
	{
		localTime = 0;

		shoot(players, gameProjectiles);
	}



	if (glm::distance(location, glm::vec2()) <= distanceToPlayer)
	{
		orbit = true;

	}

	if (orbit)
	{
		glm::vec2 newDir = glm::vec2() - location;
		glm::vec2 normal = glm::normalize(newDir);
		velocity = glm::vec2(normal.y, -normal.x);
	}

	if (location.y < distanceToPlayer)
		boundary = true;

	//int target = rand() % (2);
	if (boundary)
	{
		if (location.x >= 20)
			setLocation(20, location.y);
		if (location.x <= -20)
			setLocation(-20, location.y);
		if (location.y >= 15)
			setLocation(location.x, 15);
		if (location.y <= -15)
			setLocation(location.x, -15);		
		
	}

		move(velocity.x * .4f, velocity.y * .4f);
	//if ((location.x >= 20.0f) || (location.x <= -20.0f))
	//	velocity.x = -velocity.x;

	//for (int i = 0; i < getProjectiles().size(); i++)
	//{
	//	projectiles[i]->move(projectiles[i]->getVelocity().x, projectiles[i]->getVelocity().y);
	//
	//	if(players[0]->getTransform() && players[1]->getTransform())
	//	{
	//		if (projectiles[i]->collide(players[1]->shield))
	//		{
	//			projectiles.erase(projectiles.begin() + i);
	//			break;
	//		}
	//	}
	//
	//	if(projectiles[i]->isOffscreen())
	//	{
	//		projectiles.erase(projectiles.begin() + i);
	//		break;
	//	}
	//}
}

void OrbitEnemy::shoot(std::vector<Player*> players, std::vector<Projectile*> *gameProjectiles)
{
	//Projectile pointer to be pushed back into the vector of projectiles
	Projectile* temp = new Projectile();

	//Set the mesh of the new projectile
	temp->mesh = projectile.mesh;

	temp->mat = projectile.mat;

	//Make the new projectile's starting location equal to the player's 
	temp->setLocation(location.x, location.y);

	//Choose a random player to aim at
	int target = 0;

	if (players[0]->isAlive() && players[1]->isAlive())
		target = rand() % (players.size());

	else if (players[0]->isAlive())
		target = 0;

	else
		target = 1;

	//Normalize the projectile's velocity vector so that projectiles will fire at the same speed regardless of the amount of tilt amount
	glm::vec2 enemyToPlayer = glm::vec2(players[target]->location.x - location.x, players[target]->location.y - location.y);

	float length = sqrt((enemyToPlayer.x * enemyToPlayer.x) + (enemyToPlayer.y * enemyToPlayer.y));
	enemyToPlayer.x /= length;
	enemyToPlayer.y /= length;

	if (enemyToPlayer.x <= 0.0f)
	{
		temp->rotate = glm::rotate(temp->rotate, acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	else
	{
		temp->rotate = glm::rotate(temp->rotate, -acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	//Assign velovity
	temp->velocity = glm::vec2(0.2 * enemyToPlayer.x, 0.2 * enemyToPlayer.y);

	gameProjectiles->push_back(temp);
}