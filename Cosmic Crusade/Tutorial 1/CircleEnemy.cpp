#include "CircleEnemy.h"
#include <iostream>
#include <time.h>

CircleEnemy::CircleEnemy()
{
	velocity = glm::vec2(0.1f, -0.025f);
	delay = 1.5f;
	target.x = (rand() % 30) - 15;
	target.y = (rand() % 30) - 15;
}

CircleEnemy::~CircleEnemy()
{

}

void CircleEnemy::update(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	std::srand(time(NULL));
	//Used for shooting delay
	updateTimer->tick();
	localTime += updateTimer->getElapsedTimeS();

	lerp(target.x, target.y, (localTime / delay));
	//Shoot after they delay, and reset delay
	if (localTime > delay)
	{
		localTime = 0;

		//float temp_x = (rand() % 40) - 20;
		//target.x = temp_x - location.x;
		//
		//float temp_y = (rand() % 40) - 20;
		//target.y = temp_y - location.y;
		//
		//float mag = sqrt((target.x * target.x) + (target.y * target.y));
		target.x = (rand() % 30) - 15;
		target.y = (rand() % 30) - 15;

		//target.x /= mag;
		//target.y /= mag;

		shoot(players, gameProjectiles);
	}

	//move(target.x * 0.02, target.y * 0.02);



	//std::cout << localTime / 2.0f << std::endl;
	
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
	//std::cout << dt << std::endl;
	glm::vec2 pos = glm::vec2(((1.0f - dt) * location.x) + (dt * x), ((1.0f - dt) * location.y) + (dt * y));

	setLocation(pos.x, pos.y);

}