#pragma once
#include "GameObject.h"
#include <iostream>
#include <vector>
#include <fstream>
class Foreground {
public:
	Foreground();
	~Foreground();
	void Intialize();
	void Update(float dt);
	void UpdatePieces();
	void draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, std::vector<Light> pointLights);
	void LoadLevel(std::string fileName);
private:
	std::vector<GameObject*> Pieces;
	std::vector<char>piece;
	GameObject one, two, three, four;
	std::vector<GameObject> assets;
	std::fstream myFile;
	float time;
	float delay = 0.45f;
	float speed = 0.5f;
	float offsetX = 10;
	float scale = 1.5f;
	int increment = 4;
	const int numberOfAssets = 3;
	int assetLength;
	int assetIncrement;
};
