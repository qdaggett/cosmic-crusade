#include <iostream> // for std::cout
#include <glm\gtc\random.hpp> // for glm::linearRand


#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
	: m_pParticles(nullptr),
	m_pNumParticles(0),
	playing(true),
	interpolateColour(false)
{

}

ParticleEmitter::~ParticleEmitter()
{
	freeMemory();
}

void ParticleEmitter::initialize(unsigned int numParticles)
{
	freeMemory(); // destroy any existing particles

	if (numParticles > 0)
	{
		m_pParticles = new Particle[numParticles];
		memset(m_pParticles, 0, sizeof(Particle) * numParticles);
		m_pNumParticles = numParticles;
	}

	AttributeDescriptor posAttrib;
	posAttrib.attributeLocation = VERTEX;
}

void ParticleEmitter::freeMemory()
{
	if (m_pParticles) // if not a null pointer
	{
		delete[] m_pParticles;
		m_pParticles = nullptr;
		m_pNumParticles = 0;
	}
}

void ParticleEmitter::update(float dt)
{
	if (m_pParticles && playing) // make sure memory is initialized and system is playing
	{
		// loop through each particle
		Particle* particle = m_pParticles;
		for (unsigned int i = 0; i < m_pNumParticles; ++i, ++particle)
		{
			if (particle->life <= 0) // if particle has no life remaining
			{
				// Respawn particle
				// Note: we are not freeing memory, we are "Recycling" the particles
				particle->acceleration = glm::vec3(0.0f);
				float randomTval = glm::linearRand(0.0f, 1.0f);
				particle->colour = Math::lerp(colour0, colour1, randomTval);
				particle->life = Math::lerp(lifeRange.x, lifeRange.y, randomTval);
				particle->mass = Math::lerp(massRange.x, massRange.y, randomTval);
				particle->position = emitterPosition;
				particle->size = Math::lerp(sizeRange.x, sizeRange.y, randomTval);
				particle->velocity = Math::lerp(velocity0, velocity1, randomTval);
			}
			
			// Update physics

			// Update acceleration
			particle->acceleration = particle->force / particle->mass;
			particle->velocity = particle->velocity + (particle->acceleration * dt);
			particle->position = particle->position + particle->velocity * dt;

			// We've applied the force, let's remove it so it does not get applied next frame
			particle->force = glm::vec3(0.0f);

			// Decrease particle life
			particle->life -= dt;

			// Update visual properties
			if (interpolateColour)
			{
				// calculate t value
				float tVal = Math::invLerp(particle->life, lifeRange.x, lifeRange.y);
				particle->colour = Math::lerp(colour0, colour1, tVal);
			}
		}
	}
}

void ParticleEmitter::draw()
{
	Particle* p = m_pParticles;

	for (int i = 0; i < m_pNumParticles; ++i, ++p)
	{
		if (p->life >= 0.0f) // if particle is alive, draw it
		{
			
		}
	}
}

void ParticleEmitter::applyForceToParticle(unsigned int idx, glm::vec3 force)
{
	if (idx >= m_pNumParticles)
	{
		std::cout << "ParticleEmitter::applyForceToParticle ERROR: idx " << idx << "out of range!" << std::endl;
		return;
	}

	m_pParticles[idx].force = force;
}

glm::vec3 ParticleEmitter::getParticlePosition(unsigned int idx)
{
	if (idx >= m_pNumParticles)
	{
		std::cout << "ParticleEmitter::getParticlePosition ERROR: idx " << idx << "out of range!" << std::endl;
		return glm::vec3();
	}

	return m_pParticles[idx].position;
}
