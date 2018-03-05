#pragma once
#include <glm\glm.hpp>
#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include "ShaderProgram.h"

class Collider
{
public:
	struct BoxCollider {
		glm::vec3 center;
		glm::vec3 size;
		glm::vec3 extents;
	};

	struct SphereCollider
	{
		glm::vec3 center;
		float radius;
	};

	enum ColliderType
	{
		NONE = 0,
		BOX,
		SPHERE
	};

	Collider();
	Collider(ColliderType type, float radius);
	Collider(ColliderType type, glm::vec3 size);


	void ColliderUpdate(glm::vec3& center);

	void DisplayCollider();

	bool Collide(Collider& type);

	bool BoxBoxCollision(BoxCollider obj, BoxCollider obj2);

	bool BoxSphereCollision(BoxCollider obj, SphereCollider obj2);

	bool SphereSphereCollision(SphereCollider obj, SphereCollider obj2);

	//I couldn't think of any other way atm.
	//virtual bool Collide(Collider& obj) = 0;

	//virtual bool Collide(BoxCollider& obj) = 0;

	//virtual bool Collide(SphereCollider& obj) = 0;



	ColliderType collider;

	BoxCollider boxCollider;
	SphereCollider sphereCollider;
	
private:
	std::vector<float>boxColliderVertices;
	GLuint vboVertices, vao;

};
