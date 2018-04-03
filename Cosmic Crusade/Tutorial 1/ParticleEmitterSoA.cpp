#include "ParticleEmitterSoA.h"
#include <algorithm>
#include "glm/gtc/random.hpp"
#include <glm\gtc\matrix_transform.hpp>

ParticleEmitterSoA::ParticleEmitterSoA()
{
	// Initialize memory pointers to null
	particles.positions = nullptr;
	particles.velocities = nullptr;
	particles.remainingLives = nullptr;
	particles.accelerations = nullptr;
	particles.masses = nullptr;
	numParticles = 0;

	playing = false;
}

ParticleEmitterSoA::~ParticleEmitterSoA()
{
	freeMemory();
}


void ParticleEmitterSoA::initialize(unsigned int newNumParticles)
{
	// Clear existing memory
	if (allocated)
		freeMemory();

	if (newNumParticles) // make sure new array size is not 0
	{
		particles.positions = new glm::vec3[newNumParticles];
		particles.velocities = new glm::vec3[newNumParticles];
		particles.accelerations = new glm::vec3[newNumParticles];
		particles.remainingLives = new float[newNumParticles];
		particles.masses = new float[newNumParticles];
		particles.created = new bool[newNumParticles];

		memset(particles.remainingLives, 0, sizeof(float) * newNumParticles);
		memset(particles.created, false, sizeof(bool) * newNumParticles);

		numParticles = newNumParticles;
		allocated = true; // mark that memory has been allocated

		// Set up VBO
		// **** LOOK HERE ****
		AttributeDescriptor posAttrib;
		posAttrib.attributeLocation = VERTEX;
		posAttrib.data = particles.positions; // data points to particles.positions
		posAttrib.elementSize = sizeof(float);
		posAttrib.elementType = GL_FLOAT;
		posAttrib.numElements = newNumParticles * 3; 
		posAttrib.numElementsPerAttrib = 3;
		
		vbo.addAttributeArray(posAttrib);

		vbo.primitiveType = GL_POINTS;

		vbo.createVBO(GL_DYNAMIC_DRAW);
 	}
}

// Particle effect for ship explosions
void ParticleEmitterSoA::explosionInit(glm::vec3 pos)
{
	// Init particle emitter
	// Set the emitter properties
	lifeRange = glm::vec3(0.5f, 1.0f, 0.0f);
	initialForceMin = glm::vec3(-15.0f, -15.0f, 0.0f);
	initialForceMax = glm::vec3(15.0f, 15.0f, 0.0f);

	initialPosition = pos;
	initialize(100);
	play();
}

void ParticleEmitterSoA::laserInit(glm::vec3 pos)
{
	// Init particle emitter
	// Set the emitter properties
	lifeRange = glm::vec3(0.5f, 1.0f, 0.0f);
	initialForceMin = glm::vec3(-4.0f, -50.0f, 0.0f);
	initialForceMax = glm::vec3(4.0f, -60.0f, 0.0f);

	initialPosition = pos;
	initialize(1000);
	loop = true;
	play();
}


// Particle effect 
void ParticleEmitterSoA::sparkleInit(glm::vec3 pos)
{
	lifeRange = glm::vec3(0.2f, 0.5f, 0.0f);
	initialForceMin = glm::vec3(-7.0f, -7.0f, 0.0f);
	initialForceMax = glm::vec3(7.0f, 7.0f, 0.0f);

	initialPosition = pos;
	loop = true;
	initialize(15);
	play();
}


void ParticleEmitterSoA::update(float dt)
{


	// Make sure particle memory is initialized
	if (allocated && playing)
	{


		// loop through each particle
		for (int i = 0; i < numParticles; i++)
		{
			// Get each property for the particle using pointer arithmetic
			glm::vec3* pos = particles.positions + i;
			glm::vec3* vel = particles.velocities + i;
			glm::vec3* accel = particles.accelerations + i;
			float* life = particles.remainingLives + i;
			float* mass = particles.masses + i;
			bool *created = particles.created + i;
			// other properties... 

			// check if alive
			if ((*life <= 0))
			{
				if (*created && !loop)
				{
					*pos = glm::vec3(3000);
					sentinel++;

					if (sentinel == numParticles)
						playing = false;
				}

				else
				{
					*created = true;

					// if dead respawn
					// could put additional logic here...
					*pos = initialPosition;

					(*vel).x = glm::mix(initialForceMin.x, initialForceMax.x, glm::linearRand(0.0f, 1.0f));
					(*vel).y = glm::mix(initialForceMin.y, initialForceMax.y, glm::linearRand(0.0f, 1.0f));
					(*vel).z = glm::mix(initialForceMin.z, initialForceMax.z, glm::linearRand(0.0f, 1.0f));

					*life = glm::linearRand(lifeRange.x, lifeRange.y);
					*mass = glm::linearRand(0.5f, 1.0f);
					*accel = *vel / *mass;
				}
			}

			// Update position and velocity
			*pos += *vel * dt + *accel * 0.5f * (dt*dt);
			*vel += dt;
			*life -= dt;
		}
	}


}

void ParticleEmitterSoA::draw(glm::mat4 cameraTransform, glm::mat4 cameraProjection, ShaderProgram* shader)
{
	//Update data in VBO
	AttributeDescriptor* attrib = vbo.getAttributeDescriptor(VERTEX);

	glBindVertexArray(vbo.getVAO());

	glEnableVertexAttribArray(attrib->attributeLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO(VERTEX));
	glBufferSubData(GL_ARRAY_BUFFER, 0, numParticles * 3 * sizeof(float), particles.positions);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->sendUniformMat4("u_mv", glm::value_ptr(cameraTransform), false);
	shader->sendUniformMat4("u_proj", glm::value_ptr(cameraProjection), false);
	shader->sendUniformMat4("u_mvp", glm::value_ptr(cameraProjection * cameraTransform), false);

	texture.bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	vbo.draw(GL_POINTS);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	texture.unbind();
}

void ParticleEmitterSoA::freeMemory()
{
	// Free up all arrays here
	if (allocated)
	{
		delete[] particles.positions;
		delete[] particles.velocities;
		delete[] particles.remainingLives;
		delete[] particles.accelerations;
		delete[] particles.masses;

		particles.positions = nullptr;
		particles.velocities = nullptr;
		particles.remainingLives = nullptr;
		particles.accelerations = nullptr;
		particles.masses = nullptr;

		allocated = false;
		numParticles = 0;
	}
}

