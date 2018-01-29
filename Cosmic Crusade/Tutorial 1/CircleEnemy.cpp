#include "CircleEnemy.h"
#include <iostream>

CircleEnemy::CircleEnemy()
{
	velocity = glm::vec2(0.1f, -0.025f);
	delay = 2.0f;
	target.x = (rand() % 40) - 20;
	target.y = (rand() % 40) - 20;
}

CircleEnemy::~CircleEnemy()
{

}

void CircleEnemy::update(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	//Used for shooting delay
	updateTimer->tick();
	localTime += updateTimer->getElapsedTimeS();

	//Shoot after they delay, and reset delay
	if (localTime > delay)
	{
		localTime = 0;

		target.x = (rand() % 40) - 20;
		target.y = (rand() % 40) - 20;

		shoot(players, gameProjectiles);
	}

	std::cout << localTime << std::endl;
	lerp(target.x, target.y, localTime / 2);
}

void CircleEnemy::shoot(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	float increment = 3.14f / 4.0f;
	for (float i = 0; i < 8; i++)
	{
		Projectile* temp = new Projectile();
		temp->mesh = projectile.mesh;
		temp->mat = projectile.mat;

		temp->location = glm::vec2(0.0f, 0.0f);
		temp->move(location.x, location.y);


		glm::vec2 dir = glm::vec2(cos(increment * i), sin(increment * i));
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

		//Assign velovity
		temp->velocity = glm::vec2(0.4 * dir.x, 0.4 * dir.y);

		gameProjectiles->push_back(temp);
	}
}

void CircleEnemy::lerp(float x, float y, float dt)
{
	glm::vec2 pos = glm::vec2(((1 - dt) * location.x) + (dt * x), ((1 - dt) * location.y) + (dt * y));

	setLocation(pos.x, pos.y);
}
