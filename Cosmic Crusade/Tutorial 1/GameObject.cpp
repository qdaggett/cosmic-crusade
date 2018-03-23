#include <iostream>
#include "GameObject.h"

//GameObject::GameObject()
//{
//	location = glm::vec2(0.0f, 0.0f);
//}

//Location default is (0, 0), radius default is 0.15
GameObject::GameObject(glm::vec2 loc, float rad) : radius(rad)
{
	move(loc.x, loc.y);
	collider = new Collider();
}

GameObject::~GameObject()
{

}

//Change the object's location and the object mesh location
void GameObject::move(float x, float y)
{
	location.x += x;
	location.y += y;

	translate = glm::translate(translate, glm::vec3(x, y, 0.0f));
	transform = glm::translate(translate, glm::vec3(0.0f, 0.0f, 0.0f)) * rotate * scale;
}

void GameObject::move(float x, float y, float z)
{
	location.x += x;
	location.y += y;

	translate = glm::translate(translate, glm::vec3(x, y, z));
	transform = glm::translate(translate, glm::vec3(0.0f, 0.0f, 0.0f)) * rotate * scale;
}

void GameObject::setLocation(float x, float y)
{
	translate = glm::translate(translate, glm::vec3(-location.x, -location.y, 0.0f));

	location.x = 0.0f;
	location.y = 0.0f;

	move(x, y);
}

void GameObject::setLocation(float x, float y, float z)
{
	translate = glm::translate(translate, glm::vec3(-location.x, -location.y, 0.0f));

	location.x = 0.0f;
	location.y = 0.0f;

	move(x, y, z);
}

//Check if two objects are intersecting, just using collisions spheres for now
bool GameObject::collide(GameObject other)
{
	glm::vec2 temp = glm::vec2(location.x - other.location.x, location.y - other.location.y);
	float mag = sqrt((temp.x * temp.x) + (temp.y * temp.y));

	if (mag < (radius + other.radius))
	{
		return true;
	}

	return false;
}

void GameObject::draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, std::vector<Light> pointLights)
{
	//shader.bind();
	//tex.bind();
	if (collider->collider != Collider::NONE)
		//collider->DisplayCollider();

	shader.sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader.sendUniformMat4("uView", glm::value_ptr(cameraTransform), false);
	shader.sendUniformMat4("uProj", glm::value_ptr(cameraProjection), false);

	//Texture
	shader.sendUniform("material.diffuse", 0);
	shader.sendUniform("material.specular", 1);
	shader.sendUniform("material.hue", mat.hue);
	shader.sendUniform("material.specularExponent", mat.specularExponent);



	for (int i = 0; i < pointLights.size(); i++)
	{
		std::string prefix = ("pointLights[" + std::to_string(i));

		shader.sendUniform(prefix + "].position", cameraTransform * pointLights[i].position);
		shader.sendUniform(prefix + "].ambient", pointLights[i].ambient);
		shader.sendUniform(prefix + "].diffuse", pointLights[i].diffuse);
		shader.sendUniform(prefix + "].specular", pointLights[i].specular);

		shader.sendUniform(prefix + "].specularExponent", pointLights[i].specularExponent);
		shader.sendUniform(prefix + "].constantAttenuation", pointLights[i].constantAttenuation);
		shader.sendUniform(prefix + "].linearAttenuation", pointLights[i].linearAttenuation);
		shader.sendUniform(prefix + "].quadraticAttenuation", pointLights[i].quadraticAttenuation);
	}

	glActiveTexture(GL_TEXTURE0);
	mat.diffuse.bind();

	//glActiveTexture(GL_TEXTURE1);
	//mat.specular.bind();

	glBindVertexArray(mesh.vao);

	glDrawArrays(GL_TRIANGLES, 0, mesh.getNumVertices());
	glBindVertexArray(GL_NONE);

	//mat.specular.unbind();

	//glActiveTexture(GL_TEXTURE0);
	mat.diffuse.unbind();


	//tex.unbind();
	//shader.unbind();
}

void GameObject::draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, Light pointLight)
{
	if (collider->collider != Collider::NONE)
		//collider->DisplayCollider();
	//shader.bind();
	//tex.bind();
	shader.sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader.sendUniformMat4("uView", glm::value_ptr(cameraTransform), false);
	shader.sendUniformMat4("uProj", glm::value_ptr(cameraProjection), false);

	//Texture
	shader.sendUniform("material.diffuse", 0);
	shader.sendUniform("material.specular", 1);
	shader.sendUniform("material.hue", mat.hue);
	shader.sendUniform("material.specularExponent", mat.specularExponent);


	std::string prefix = "pointLight.";
	shader.sendUniform(prefix + "position", cameraTransform * pointLight.position);
	shader.sendUniform(prefix + "ambient", pointLight.ambient);
	shader.sendUniform(prefix + "diffuse", pointLight.diffuse);
	shader.sendUniform(prefix + "specular", pointLight.specular);

	shader.sendUniform(prefix + "specularExponent", pointLight.specularExponent);
	shader.sendUniform(prefix + "constantAttenuation", pointLight.constantAttenuation);
	shader.sendUniform(prefix + "linearAttenuation", pointLight.linearAttenuation);
	shader.sendUniform(prefix + "quadraticAttenuation", pointLight.quadraticAttenuation);


	glActiveTexture(GL_TEXTURE0);
	mat.diffuse.bind();

	//glActiveTexture(GL_TEXTURE1);
	//mat.specular.bind();

	glBindVertexArray(mesh.vao);

	glDrawArrays(GL_TRIANGLES, 0, mesh.getNumVertices());
	glBindVertexArray(GL_NONE);

	//mat.specular.unbind();

	//glActiveTexture(GL_TEXTURE0);
	mat.diffuse.unbind();
	//tex.unbind();
	//shader.unbind();

}


void GameObject::loadTexture(TextureType type, const std::string & texFile)
{
	mat.loadTexture(type, texFile);
}

void GameObject::lookAt(glm::vec3 & position, glm::vec3 & target, glm::vec3 up)
{
	glm::vec3 direction = glm::normalize(target - position);
	glm::vec3 right = glm::normalize(glm::cross(up, direction));

	up = glm::cross(direction, right);

	rotate = glm::mat4(right.x, up.x, direction.x, position.x,
		right.y, up.y, direction.y, position.y,
		right.z, up.z, direction.z, position.z,
		0, 0, 0, 1);
}

