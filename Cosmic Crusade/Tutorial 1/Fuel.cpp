#include "Fuel.h"

Fuel::Fuel()
{

}

Fuel::~Fuel()
{

}

void Fuel::initializePowerUp(std::vector<ParticleEmitterSoA*>* emitter)
{
	//emitters = emitter;

	// Creating collider for ammo powerup
	collider = new Collider(Collider::SPHERE, 0.8f);

	// Setting mesh
	if (!mesh.loadFromFile("meshes/BombPowerUp.obj"))
	{
		std::cout << "Fuel powerup model failed to load." << std::endl;
		system("pause");
		exit(0);
	}
}

void Fuel::updatePowerUp(std::vector<Player*> players, float time)
{
	// Setting up timer and updating collider
	this->collider->ColliderUpdate(glm::vec3(location, 0));
	localTime += time;
	rotationTick += time;
	//ParticleEmitterSoA* fuel = new ParticleEmitterSoA();
	//fuel->explosionInit(glm::vec3(-location.x, location.y, 1.0f));
	//mat.loadTexture(Diffuse, "Textures/blue.png");
	//fuel->texture = mat.diffuse;
	//emitters->push_back(fuel);

	if (powerupIsActive == true)
	{
		setLocation(spawnPosition.x, spawnPosition.y);
		
	}

	if (this->collider->Collide(*players[0]->collider))
	{
		collected = true;
		players[1]->addSpeedUp(20);
		setLocation(999, 999);
		localTime = 0.0f;
		powerupIsActive = false;
	}

	if (this->collider->Collide(*players[1]->collider))
	{
		collected = true;
		players[0]->addSpeedUp(20);
		setLocation(999, 999);
		localTime = 0.0f;
		powerupIsActive = false;
	}

	rotate = glm::rotate(this->ogRotate, rotationTick * (glm::pi<float>() / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	move(0, 0);
}

void Fuel::drawPowerUp(ShaderProgram &shader, glm::mat4 camera, glm::mat4 projection, std::vector<Light> lights)
{
	draw(shader, camera, projection, lights);
}

void Fuel::spawnPowerUp(glm::vec2 position)
{
	spawnPosition = position;
	powerupIsActive = true;
}