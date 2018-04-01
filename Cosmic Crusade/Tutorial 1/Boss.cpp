#include "Boss.h"

Boss::Boss()
{
	lerpTarget.x = 0;
	lerpTarget.y = 0;

	hitPoints = 50;

	collider = new Collider(Collider::BOX, glm::vec3(5, 10, 0));

	rotate = glm::rotate(ogRotate, 89.5f, glm::vec3(1, 0, 0));
}

Boss::~Boss()
{

}

void Boss::update(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	collider->ColliderUpdate(glm::vec3(location.x, location.y, 0));
	updateTime();

	if (localTime > basicDelay)
	{
		shootBasic(players, gameProjectiles);

		localTime = 0.0f;
	}

	if (spinTime > spinDelay)
	{
		shoot(players, gameProjectiles);

		spinTime = 0.0f;
	}

	if (circleTime > circleDelay)
	{
		shootCircle(players, gameProjectiles);

		circleTime = 0.0f;
	}

	if (lerpTime <= 1.0f)
		lerp(lerpTarget.x, lerpTarget.y, lerpTime);
}

void Boss::shoot(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	shootAng += (updateTimer->getElapsedTimeS() * 8);

	Projectile* temp = new Projectile();
	temp->mesh = projectile.mesh;
	temp->mat = projectile.mat;
	temp->collider = new Collider(Collider::SPHERE, 0.5f);

	temp->location = glm::vec3(0.0f, 0.0f, 0.0f);
	temp->move(location.x, location.y);

	glm::vec2 dir = glm::vec2(cos(shootAng), sin(shootAng));
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

void Boss::shootBasic(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	//Projectile pointer to be pushed back into the vector of projectiles
	Projectile* temp = new Projectile();

	//Set the mesh of the new projectile
	temp->mesh = projectile.mesh;

	temp->mat = projectile.mat;

	temp->collider = new Collider(Collider::SPHERE, 0.5f);

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
	}

	else
	{
		temp->rotate = glm::rotate(temp->rotate, -acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	//Assign velovity
	temp->velocity = glm::vec2(0.4 * enemyToPlayer.x, 0.4 * enemyToPlayer.y);

	gameProjectiles->push_back(temp);
}

void Boss::shootCircle(std::vector<Player*> players, std::vector<Projectile*>* gameProjectiles)
{
	float increment = 3.14f / 4.0f;
	for (float i = 0; i < 8; i++)
	{
		Projectile* temp = new Projectile();
		temp->mesh = projectile.mesh;
		temp->mat = projectile.mat;

		temp->collider = new Collider(Collider::SPHERE, 0.5f);

		temp->location = glm::vec3(0.0f, 0.0f, 0.0f);
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

void Boss::updateTime()
{
	updateTimer->tick();
	localTime += updateTimer->getElapsedTimeS();
	lerpTime += (updateTimer->getElapsedTimeS() / 4);
	spinTime += updateTimer->getElapsedTimeS();
	circleTime += updateTimer->getElapsedTimeS();

}