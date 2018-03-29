#include "EnemyManager.h"
// 15/3/2018
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Intialize(std::vector<Player*> players)
{
	timer = 0;

	this->players = players;

	//Load enemy models
	if (!basicEnemy.mesh.loadFromFile("meshes/Basic_Enemy.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!circleEnemy.mesh.loadFromFile("meshes/sphere.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!orbitEnemy.mesh.loadFromFile("meshes/sphere.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!laserEnemy.mesh.loadFromFile("meshes/Laser.obj"))
	{
		std::cout << "Laser enemy model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!boss.mesh.loadFromFile("meshes/Boss.obj"))
	{
		std::cout << "Boss model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	//Load bullets
	if (!enemyBullet.mesh.loadFromFile("meshes/sphere.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}


	enemyBullet.scale = glm::scale(enemyBullet.scale, glm::vec3(0.25f));

	//Set the enemy meshes 
	basicEnemy.projectile.mesh = enemyBullet.mesh;

	circleEnemy.projectile.mesh = enemyBullet.mesh;

	orbitEnemy.projectile.mesh = enemyBullet.mesh;

	boss.projectile.mesh = enemyBullet.mesh;

	//Load the textures for enemies

	basicEnemy.loadTexture(Diffuse, "Textures/Basic_Enemy.png");

	red.loadTexture(Diffuse, "Textures/red.png");

	blue.loadTexture(Diffuse, "Textures/blue.png");

	purple.loadTexture(Diffuse, "Textures/purple.png");

	damaged.loadTexture(Diffuse, "Textures/NotWhite.png");

	//Set their material
	basicEnemy.defaultMaterial = basicEnemy.mat;
	basicEnemy.projectile.mat = purple;

	circleEnemy.mat = red;
	circleEnemy.defaultMaterial = circleEnemy.mat;
	circleEnemy.projectile.mat = purple;

	orbitEnemy.mat = red;
	orbitEnemy.defaultMaterial = orbitEnemy.mat;
	orbitEnemy.projectile.mat = purple;

	laserEnemy.mat = red;
	laserEnemy.defaultMaterial = laserEnemy.mat;

	boss.mat = red;
	boss.defaultMaterial = boss.mat;
	boss.projectile.mat = purple;

	LoadLevel();
}

void EnemyManager::Update(float dt)
{
	timer += dt;

	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->gotDamaged && enemyList[i]->damagedTimer <= damagedTimer)
		{
			enemyList[i]->mat = damaged;
			enemyList[i]->damagedTimer += dt;
		}
		else if (enemyList[i]->damagedTimer >= damagedTimer)
		{
			enemyList[i]->gotDamaged = false;
			enemyList[i]->damagedTimer = 0;
			enemyList[i]->mat = enemyList[i]->defaultMaterial;
		}

		enemyList[i]->update(players, &enemyProjectiles);

		if (enemyList[i]->location.y <= -20)
		{
			enemyList.erase(enemyList.begin() + i);
			break;
		}

		if (enemyList[i]->location.x > 35 || enemyList[i]->location.x < -35 || enemyList[i]->location.y > 35 || enemyList[i]->location.y < -35)
		{
			enemyList.erase(enemyList.begin() + i);
			break;
		}
	}
	//std::cout << timer << std::endl;
	SpawnEnemy();
	UpdateEnemyProjectile();

}

void EnemyManager::Draw(ShaderProgram &shader, glm::mat4 camera, glm::mat4 projection, std::vector<Light> lights)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->draw(shader, camera, projection, lights);
	}

	for (int i = 0; i < enemyProjectiles.size(); i++)
	{
		enemyProjectiles[i]->draw(shader, camera, projection, lights);
	}
}


void EnemyManager::SpawnEnemy()
{

	if (count < spawnList.size() && spawnList[count]->spawnTime <= timer)
	{
		if (spawnList[count]->type == Basic)
		{
			BasicEnemy* temp = new BasicEnemy();
			temp->mesh = basicEnemy.mesh;
			temp->mat = basicEnemy.mat;
			temp->defaultMaterial = basicEnemy.mat;
			temp->projectile.mesh = basicEnemy.projectile.mesh;
			temp->projectile.mat = basicEnemy.projectile.mat;
			temp->setLocation(spawnList[count]->position.x, spawnList[count]->position.y);
			temp->moveSideways = false;
			temp->Intialize();

			enemyList.push_back(temp);

		}
		if (spawnList[count]->type == BasicMove)
		{
			BasicEnemy* temp = new BasicEnemy();
			temp->mesh = basicEnemy.mesh;
			temp->mat = basicEnemy.mat;
			temp->defaultMaterial = basicEnemy.mat;
			temp->projectile.mesh = basicEnemy.projectile.mesh;
			temp->projectile.mat = basicEnemy.projectile.mat;
			temp->setLocation(spawnList[count]->position.x, spawnList[count]->position.y);
			temp->moveSideways = true;
			temp->Intialize();

			enemyList.push_back(temp);
		}
		if (spawnList[count]->type == Circle)
		{
			CircleEnemy* temp = new CircleEnemy();
			temp->mesh = circleEnemy.mesh;
			temp->mat = circleEnemy.mat;
			temp->defaultMaterial = circleEnemy.mat;
			temp->projectile.mesh = circleEnemy.projectile.mesh;
			temp->projectile.mat = circleEnemy.projectile.mat;

			temp->setLocation(spawnList[count]->position.x, spawnList[count]->position.y);

			enemyList.push_back(temp);

		}
		if (spawnList[count]->type == Orbit)
		{
			OrbitEnemy* temp = new OrbitEnemy();
			temp->mesh = orbitEnemy.mesh;
			temp->mat = orbitEnemy.mat;
			temp->defaultMaterial = orbitEnemy.mat;
			temp->projectile.mesh = orbitEnemy.projectile.mesh;
			temp->projectile.mat = orbitEnemy.projectile.mat;

			temp->setLocation(spawnList[count]->position.x, spawnList[count]->position.y);

			enemyList.push_back(temp);
		}

		if (spawnList[count]->type == Laser)
		{
			LaserEnemy* temp = new LaserEnemy();
			temp->mesh = laserEnemy.mesh;
			temp->mat = laserEnemy.mat;
			temp->defaultMaterial = laserEnemy.mat;
			temp->projectile.mesh = laserEnemy.projectile.mesh;
			temp->projectile.mat = laserEnemy.projectile.mat;

			temp->setLocation(spawnList[count]->position.x, spawnList[count]->position.y);

			//temp->laserObject.mesh = orbitEnemy.mesh;
			temp->Intialize(true);

			enemyList.push_back(temp);
		}

		if (spawnList[count]->type == BOSS)
		{
			Boss* temp = new Boss();

			temp->mesh = boss.mesh;
			temp->mat = boss.mat;
			temp->defaultMaterial = boss.mat;
			temp->projectile.mesh = boss.projectile.mesh;
			temp->projectile.mat = boss.projectile.mat;

			temp->setLocation(spawnList[count]->position.x, spawnList[count]->position.y);

			bossSpawn = true;

			enemyList.push_back(temp);
		}
		count++;
	}

}

void EnemyManager::LoadLevel()
{
	//spawnList.push_back(new EnemyNode(5, glm::vec2(0, 20), Circle));
	spawnList.push_back(new EnemyNode(1.f, glm::vec2(0, 0), BOSS));
	//spawnList.push_back(new EnemyNode(5.2f, glm::vec2(30, 0), Circle));
	//spawnList.push_back(new EnemyNode(2, glm::vec2(-10, 20), Orbit));

	//spawnList.push_back(new EnemyNode(12, glm::vec2(-30, -10), BasicMove));
	//spawnList.push_back(new EnemyNode(12, glm::vec2(30, 10), BasicMove));
	//
	//spawnList.push_back(new EnemyNode(20, glm::vec2(-30, 20), BasicMove));
	//spawnList.push_back(new EnemyNode(20.5f, glm::vec2(30, -20), BasicMove));
	//spawnList.push_back(new EnemyNode(21, glm::vec2(0, 20), BasicMove));
	//
	//spawnList.push_back(new EnemyNode(29, glm::vec2(-30, 0), BasicMove));
	//spawnList.push_back(new EnemyNode(30, glm::vec2(30, 0), BasicMove));
	//spawnList.push_back(new EnemyNode(31, glm::vec2(0, 20), BasicMove));
	//spawnList.push_back(new EnemyNode(32, glm::vec2(10, 20), Basic));
	//
	//spawnList.push_back(new EnemyNode(40, glm::vec2(30, 10), Basic));
	//spawnList.push_back(new EnemyNode(41, glm::vec2(-30, -10), Basic));
	//spawnList.push_back(new EnemyNode(42, glm::vec2(30, -10), Basic));
	//spawnList.push_back(new EnemyNode(43, glm::vec2(-30, 10), Basic));
	//spawnList.push_back(new EnemyNode(48, glm::vec2(10, 20), Orbit));
	//
	//spawnList.push_back(new EnemyNode(52, glm::vec2(-30, 20), Orbit));
	//spawnList.push_back(new EnemyNode(55, glm::vec2(30, -20), Orbit));
	//spawnList.push_back(new EnemyNode(60, glm::vec2(0, 20), BasicMove));
	//
	//spawnList.push_back(new EnemyNode(70, glm::vec2(-30, 20), Circle));
	//spawnList.push_back(new EnemyNode(74, glm::vec2(30, -20), Circle));
	//
	//spawnList.push_back(new EnemyNode(84, glm::vec2(-30, 20), Basic));
	//spawnList.push_back(new EnemyNode(85, glm::vec2(10, 20), Basic));
	//spawnList.push_back(new EnemyNode(85, glm::vec2(-10, 20), Basic));
	//spawnList.push_back(new EnemyNode(86, glm::vec2(-30, -15), Basic));
	//
	//spawnList.push_back(new EnemyNode(93, glm::vec2(-10, 20), Basic));
	//spawnList.push_back(new EnemyNode(95, glm::vec2(0, 20), BasicMove));
	//spawnList.push_back(new EnemyNode(97, glm::vec2(10, 20), Basic));
	//spawnList.push_back(new EnemyNode(99, glm::vec2(20, 20), Orbit));
	//spawnList.push_back(new EnemyNode(100, glm::vec2(0, 20), Orbit));
}

void EnemyManager::UpdateEnemyProjectile()
{
	for (int i = 0; i < enemyProjectiles.size(); i++)
	{
		enemyProjectiles[i]->move(enemyProjectiles[i]->getVelocity().x, enemyProjectiles[i]->getVelocity().y);
		enemyProjectiles[i]->collider->ColliderUpdate(glm::vec3(enemyProjectiles[i]->location, 0));

		if (players[0]->getTransform() && players[1]->getTransform())
		{
			if (enemyProjectiles[i]->collider->Collide(*players[1]->shield.collider))
			{
				enemyProjectiles.erase(enemyProjectiles.begin() + i);
				break;
			}
		}

		GameObject temp;
		temp.location = enemyProjectiles[i]->location;
		temp.radius = enemyProjectiles[i]->radius;
		temp.collider = enemyProjectiles[i]->collider;
		temp.scale = enemyProjectiles[i]->scale;

		if ((players[0]->collider->Collide(*temp.collider)) && (players[0]->getState() == Player::state::alive) && !players[0]->isInvulnerable())
		{
			players[0]->setDead();
			enemyProjectiles.erase(enemyProjectiles.begin() + i);
			if (players[0]->getTransform())
				Player::progress -= 1.f;
			else
				Player::progress -= 5.f;
			break;
		}


		if (players[1]->collider->Collide(*temp.collider) && (players[1]->getState() == Player::state::alive) && !players[1]->isInvulnerable())
		{
			players[1]->setDead();
			if (players[1]->getTransform())
				Player::progress -= 1.f;
			else
				Player::progress -= 5.f;

			enemyProjectiles.erase(enemyProjectiles.begin() + i);
			break;
		}


		if (enemyProjectiles[i]->isOffscreen())
		{
			enemyProjectiles.erase(enemyProjectiles.begin() + i);
			break;
		}
	}
}

void EnemyManager::Unload()
{
	circleEnemy.mesh.unload();
	basicEnemy.mesh.unload();
	orbitEnemy.mesh.unload();
	enemyList.clear();
	spawnList.clear();
	enemyProjectiles.clear();
	players.clear();

	for (int i = 0; i < enemyList.size(); i++)
	{
		enemyList.erase(enemyList.begin() + i);
	}
}
