#include "Ammo.h"

Ammo::Ammo()
{

}

Ammo::~Ammo()
{

}

void Ammo::initializePowerUp(std::vector<ParticleEmitterSoA*>* emitter)
{
	emitters = emitter;

	// Creating collider for ammo powerup
	collider = new Collider(Collider::SPHERE, 0.8f);

	// Setting mesh
	if (!mesh.loadFromFile("meshes/TwinLaser.obj"))
	{
		std::cout << "Ammo powerup model failed to load." << std::endl;
		system("pause");
		exit(0);
	}
	mat.loadTexture(Diffuse, "Textures/green.png");
}

void Ammo::updatePowerUp(std::vector<Player*> players, float time)
{
	// Setting up timer and updating collider
	this->collider->ColliderUpdate(glm::vec3(location, 0));
	localTime += time;
	rotationTick += time;

	if (powerupIsActive == true)
	{
		setLocation(spawnPosition.x, spawnPosition.y);
		if (particleHasPlayed == false)
		{
			ammoParticles->sparkleInit(glm::vec3(-spawnPosition.x, spawnPosition.y + 1.2, 1.0f));
			ammoParticles->texture = mat.diffuse;
			emitters->push_back(ammoParticles);
			particleHasPlayed = true;
		}
		
		else
		{
			ammoParticles->initialPosition = glm::vec3(-spawnPosition.x, spawnPosition.y + 1.2, 1.0f);
		}
	}

	if (this->collider->Collide(*players[0]->collider))
	{
		collected = true;
		players[1]->addAmmo(3);
		setLocation(999, 999);
		ammoParticles->initialPosition = glm::vec3(999, 999, 1.0f);
		localTime = 0.0f;
		powerupIsActive = false;
	}

	if (this->collider->Collide(*players[1]->collider))
	{
		collected = true;
		players[0]->addAmmo(3);
		setLocation(999, 999);
		ammoParticles->initialPosition = glm::vec3(999, 999, 1.0f);
		localTime = 0.0f;
		powerupIsActive = false;
	}



	rotate = glm::rotate(this->ogRotate, rotationTick * (glm::pi<float>() / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	move(0, 0);
}

void Ammo::drawPowerUp(ShaderProgram &shader, glm::mat4 camera, glm::mat4 projection, std::vector<Light> lights)
{
	draw(shader, camera, projection, lights);
}

void Ammo::spawnPowerUp(glm::vec2 position)
{
	spawnPosition = position;
	powerupIsActive = true;
}