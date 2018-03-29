#pragma once

#include "BasicEnemy.h"
#include "CircleEnemy.h"
#include "OrbitEnemy.h"
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	void Update(float dt);
	void Draw(ShaderProgram &shader, glm::mat4 camera, glm::mat4 projection, std::vector<Light> point);
	void SpawnEnemy();
	void LoadLevel();
	void Intialize(std::vector<Player*> player);
	void UpdateEnemyProjectile();
	void Unload();
	std::vector<Enemy*> getEnemyList()
	{
		return enemyList;
	};

	enum EnemyType {
		Basic = 0,
		Circle,
		Orbit,
		Laser, 
		BasicMove
	};
	struct EnemyNode {
		EnemyNode(float time, glm::vec2 pos, EnemyType type) : spawnTime(time), position(pos), type(type) {}
		glm::vec2 position;
		float spawnTime;
		EnemyType type;
	};
	std::vector<Enemy*> enemyList;
	std::vector<EnemyNode*> spawnList;

	bool playerDied;

	int count = 0;
private:

	CircleEnemy circleEnemy;
	BasicEnemy basicEnemy;
	OrbitEnemy orbitEnemy;

	GameObject enemyBullet;

	Material red, blue, purple, damaged; 

	float timer = 0;


	std::vector<Projectile*> enemyProjectiles;

	std::vector<Player*> players;

	float damagedTimer = 0.1f;
};
