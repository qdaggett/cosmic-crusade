#include "Collider.h"

Collider::Collider()
{
	collider = NONE;
}

Collider::Collider(ColliderType type, float radius)
{
	if (type == SPHERE)
	{
		collider = SPHERE;
		sphereCollider.radius = radius;
	}
	else
		std::cout << "Collider not intialized. Please intialize ColliderType to SPHERE to set the radius." << std::endl;

}

Collider::Collider(ColliderType type, glm::vec3 size)
{
	if (type == BOX)
	{
		collider = BOX;
		boxCollider.size = size;
		boxCollider.extents = glm::vec3(size.x / 2, size.y / 2, size.z / 2);
	}
	else
		std::cout << "Collider not intialized. Please intialize ColliderType to BOX to set the vector size." << std::endl;
}

void Collider::ColliderUpdate(glm::vec3& center)
{
	if (collider != NONE)
	{
		if (collider == SPHERE)
			sphereCollider.center = center;
		else if (collider == BOX)
			boxCollider.center = center;
	}
	else
		std::cout << "Can't update collider, collider has not been intialized." << std::endl;
}

void Collider::DisplayCollider()
{
	if (collider != ColliderType::NONE)
	{

		if (collider == ColliderType::BOX)
		{
			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Left, Up, Forward

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Left, Up, Back

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Left, Down, Forward

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Left, Down, Back



			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Right, Up,Forward

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Right Up, Back

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Right, Down, Forward

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Right, Down, Back



			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Right, Up,Forward

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Right Up, Back

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Left, Up, Forward

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Left, Up, Back



			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Right, Down, Forward

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Right, Down, Back

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Left, Down, Forward

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Left, Down, Back



			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Left, Up, Forward

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Left, Down, Forward

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Right, Up,Forward

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z + boxCollider.extents.z); //Right, Down, Forward



			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Left, Up, Back

			boxColliderVertices.push_back(boxCollider.center.x + boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Left, Down, Back

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y + boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Right, Up, Back

			boxColliderVertices.push_back(boxCollider.center.x - boxCollider.extents.x);
			boxColliderVertices.push_back(boxCollider.center.y - boxCollider.extents.y);
			boxColliderVertices.push_back(boxCollider.center.z - boxCollider.extents.z); //Right, Down, Back



			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vboVertices);

			glBindVertexArray(vao);

			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
			glBufferData(GL_ARRAY_BUFFER, boxColliderVertices.size() * sizeof(float*), &boxColliderVertices[0], GL_STATIC_DRAW);
			//glDrawArrays(GL_QUADS, 0, boxColliderVertices.size());
			//glVertexAttribPointer(0);

			//glBindBuffer(GL_ARRAY_BUFFER, GL_QUADS);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * boxColliderVertices.size(), &boxColliderVertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
			glBindVertexArray(GL_NONE);
			//glDrawArrays(GL_QUADS, 0, boxColliderVertices.size());
			//glBindVertexArray(GL_NONE);
			//glEnable(GL_CULL_FACE);
		}
		else if (collider == ColliderType::SPHERE)
		{

		}


		//glEnd();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

bool Collider::Collide(Collider & type)
{
	if (type.collider != ColliderType::NONE)
	{
		if (collider == ColliderType::BOX)
		{
			if (type.collider == ColliderType::BOX)
			{
				return BoxBoxCollision(boxCollider, type.boxCollider);
			}

			else if (type.collider == ColliderType::SPHERE)
			{
				return BoxSphereCollision(boxCollider, type.sphereCollider);
			}
		}
		else if (collider == ColliderType::SPHERE)
		{
			if (type.collider == ColliderType::BOX)
			{
				return BoxSphereCollision(type.boxCollider, sphereCollider);
			}

			else if (type.collider == ColliderType::SPHERE)
			{
				return SphereSphereCollision(sphereCollider, type.sphereCollider);
			}
		}
	}
	else
	{
		std::cout << "Can't check collision, collider has not been intialized." << std::endl;
		return false;
	}
}

bool Collider::BoxBoxCollision(BoxCollider obj1, BoxCollider obj2)
{
	return (obj1.center.x + obj1.extents.x >= obj2.center.x + -obj2.extents.x && obj1.center.x + -obj1.extents.x <= obj2.center.x + obj2.extents.x) &&
		(obj1.center.y + obj1.extents.y >= obj2.center.y + -obj2.extents.y && obj1.center.y + -obj1.extents.y <= obj2.center.y + obj2.extents.y) &&
		(obj1.center.z + obj1.extents.z >= obj2.center.z + -obj2.extents.z && obj1.center.z + -obj1.extents.z <= obj2.center.z + obj2.extents.z);
}

bool Collider::BoxSphereCollision(BoxCollider obj1, SphereCollider obj2)
{
	return (obj2.center.x >= (obj1.center.x - obj1.extents.x) - obj2.radius) &&
		(obj2.center.x <= (obj1.center.x + obj1.extents.x) + obj2.radius) &&
		(obj2.center.y >= (obj1.center.y - obj1.extents.y) - obj2.radius) &&
		(obj2.center.y <= (obj1.center.y + obj1.extents.y) + obj2.radius) &&
		(obj2.center.z >= (obj1.center.z - obj1.extents.z) - obj2.radius) &&
		(obj2.center.z <= (obj1.center.z + obj1.extents.z) + obj2.radius);
}

bool Collider::SphereSphereCollision(SphereCollider obj1, SphereCollider obj2)
{
	float distance = glm::distance(obj1.center, obj2.center);
	return (distance < (obj1.radius + obj2.radius));
}


