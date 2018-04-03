#include "LaserEnemy.h"					 
LaserEnemy::LaserEnemy()
{
	collider = new Collider(Collider::BOX, glm::vec3(1.5f, 1.5f, 0));
	hitPoints = 1000;
	//	laserCollider = new Collider(Collider::BOX, glm::vec3(1.5f, 1.5f, 0));
	laserObject.setLocation(location.x, location.y);
}

LaserEnemy::~LaserEnemy()
{

}

void LaserEnemy::Intialize()
{

}

void LaserEnemy::destroy()
{
	laserEmitter->loop = false;
}

void LaserEnemy::Intialize(bool move)
{
	if (location.x < 0 && location.y > 0)
	{
		quad = TopLeft;
		if (move)
		{
			moveY = true;
			rotate = glm::rotate(ogRotate, -89.5f, glm::vec3(0, 0, 1));
			glm::mat4 temp = rotate;
			laserObject.rotate = glm::rotate(temp, -89.5f, glm::vec3(0, 0, 1));
			//setLocation(location.x, location.y - 11);
			laserObject.collider = new Collider(Collider::BOX, glm::vec3(1, 100, 1));
		}
		else
		{
			setLocation(location.x - 6, location.y);
			laserObject.collider = new Collider(Collider::BOX, glm::vec3(100, 1, 1));
		}
	}
	else if (location.x > 0 && location.y > 0)
	{
		quad = TopRight;
		if (move)
		{
			moveY = true;
			rotate = glm::rotate(ogRotate, -89.5f, glm::vec3(0, 0, 1));
			//setLocation(location.x, location.y - 11);
			laserObject.collider = new Collider(Collider::BOX, glm::vec3(1, 100, 1));
		}
		else
		{
			rotate = glm::rotate(ogRotate, -172.8f, glm::vec3(0, 0, 1));
			//setLocation(location.x - 6, location.y);
			laserObject.collider = new Collider(Collider::BOX, glm::vec3(100, 1, 1));
		}
	}
	else
	{

	}

	//The stuff above this seems like it doesn't matter...
	velocity = glm::vec2(0.1f, 0.0f);
}

void LaserEnemy::update(std::vector<Player*> players, std::vector<Projectile*>*)
{
	collider->ColliderUpdate(glm::vec3(location, 0));
	laserObject.setLocation(location.x, location.y);
	laserObject.collider->ColliderUpdate(glm::vec3(laserObject.location, 0));

	//adjusting velocity based on location
	if (location.y >= 15.0f)
		velocity.y = -abs(velocity.y);

	if (location.y <= -15.0f)
		velocity.y = abs(velocity.y);

	if (location.x >= 20.0f)
		velocity.x = -abs(velocity.x);

	if (location.x <= -20.0f)
		velocity.x = abs(velocity.x);

	//Move enemy and emitter
	move(velocity.x, velocity.y);
	laserEmitter->initialPosition = glm::vec3(-location.x, location.y, 1.0f);

	if (laserObject.collider->Collide(*players[0]->collider) && players[0]->isAlive() && !players[0]->isInvulnerable())
	{
		//players[0]->mat = mat;
		players[0]->setDead();

		if (players[0]->getTransform())
			Player::progress -= 5.f;

		std::cout << "Colliding with laser" << std::endl;
	}
	if (laserObject.collider->Collide(*players[1]->collider) && players[1]->isAlive() && !players[1]->isInvulnerable())
	{
		players[1]->setDead();

		if (players[1]->getTransform())
			Player::progress -= 5.f;

		std::cout << "Colliding with laser" << std::endl;
	}
}

void LaserEnemy::shoot(std::vector<Player*>, std::vector<Projectile*>*){}