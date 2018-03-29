#include "BasicEnemy.h"

BasicEnemy::BasicEnemy()
{
	//velocity = glm::vec2(0.1f, -0.025f);

}

void BasicEnemy::Intialize()
{
	delay = 0.75f;

	if (moveSideways)
	{
		if (location.x <= -30 && location.y <= 20)
		{
			moveUpDown = true;
			velocity = glm::vec2(0.05f, 0.3f);
		}
		else if (location.x >= 30 && location.y <= 20)
		{
			moveUpDown = true;
			velocity = glm::vec2(-0.05f, 0.3f);
		}
		else
			velocity = glm::vec2(0.3f, -0.05f);
	}

	velocity = glm::vec2(0.1f, -0.025f);
	delay = 0.7f;

	collider = new Collider(Collider::SPHERE, 0.4f);
}

BasicEnemy::~BasicEnemy()
{

}

void BasicEnemy::update(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	//Used for shooting delay
	updateTimer->tick();
	this->collider->ColliderUpdate(glm::vec3(location, 0));
	localTime += updateTimer->getElapsedTimeS();

	//Shoot after they delay, and reset delay
	if (localTime > delay)
	{
		localTime = 0;

		shoot(players, gameProjectiles);
	}

	move(velocity.x, velocity.y);

	if (moveSideways)
	{
		if (moveUpDown)
		{
			if ((location.y >= 15.0f) || (location.y <= -15.0f))
				velocity.y = -velocity.y;
		}
		else
		{
			if ((location.x >= 20.0f) || (location.x <= -20.0f))
				velocity.x = -velocity.x;
		}
	}
	else
	{
		if (!stopMove)
			velocity = glm::normalize(players[target]->location - location) * 0.15f;
		if (glm::distance(location, players[target]->location) <= 12)
		{
			velocity = glm::vec2();
			stopMove = true;
		}
	}

	//Normalize the projectile's velocity vector so that projectiles will fire at the same speed regardless of the amount of tilt amount
	glm::vec2 enemyToPlayer = glm::vec2(players[target]->location.x - location.x, players[target]->location.y - location.y);

	float length = sqrt((enemyToPlayer.x * enemyToPlayer.x) + (enemyToPlayer.y * enemyToPlayer.y));
	enemyToPlayer.x /= length;
	enemyToPlayer.y /= length;

	if (enemyToPlayer.x <= 0.0f)
	{
		rotate = glm::rotate(ogRotate, acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	else
	{
		rotate = glm::rotate(ogRotate, -acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, -1.0f));
	}
}

void BasicEnemy::shoot(std::vector<Player*> players, std::vector<Projectile*> *gameProjectiles)
{
	// Toggling on boolean for shooting sound
	enemyHasShot = true;

	//Projectile pointer to be pushed back into the vector of projectiles
	Projectile* temp = new Projectile();

	//Set the mesh of the new projectile
	temp->mesh = projectile.mesh;

	temp->mat = projectile.mat;

	temp->collider = new Collider(Collider::SPHERE, 1.f);

	//Make the new projectile's starting location equal to the player's 
	temp->setLocation(location.x, location.y);

	//Choose a random player to aim at
	target = 0;

	if (players[0]->isAlive() && players[1]->isAlive())
		target = rand() % (players.size());

	else if (players[0]->isAlive())
		target = 0;

	else if (players[1]->isAlive())
		target = 1;

	else
		return;


	//Normalize the projectile's velocity vector so that projectiles will fire at the same speed regardless of the amount of tilt amount
	glm::vec2 enemyToPlayer = glm::vec2(players[target]->location.x - location.x, players[target]->location.y - location.y);

	float length = sqrt((enemyToPlayer.x * enemyToPlayer.x) + (enemyToPlayer.y * enemyToPlayer.y));
	enemyToPlayer.x /= length;
	enemyToPlayer.y /= length;

	if (enemyToPlayer.x <= 0.0f)
	{
		temp->rotate = glm::rotate(temp->rotate, acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
		rotate = glm::rotate(ogRotate, acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	else
	{
		temp->rotate = glm::rotate(temp->rotate, -acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
		rotate = glm::rotate(ogRotate, -acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	//Assign velovity
	temp->velocity = glm::vec2(0.4 * enemyToPlayer.x, 0.4 * enemyToPlayer.y);

	gameProjectiles->push_back(temp);
}