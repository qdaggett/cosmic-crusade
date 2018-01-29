#include "Foreground.h"

Foreground::Foreground()
{

}

Foreground::~Foreground()
{

}

void Foreground::Intialize()
{


	if (!one.mesh.loadFromFile("meshes/fore.obj"))
	{
		std::cout << "Foreground: Can't load model\n";
	}

	assets.resize(numberOfAssets);

	for (int i = 0; i < numberOfAssets; i++)
	{
		if (!assets[i].mesh.loadFromFile("levels/assets/background" + std::to_string(i+1) + ".obj"))
		{
			std::cout << "Background: model background" + std::to_string(i+1) + ".obj could not be loaded.\n";
		}

		assets[i].mat.loadTexture(TextureType::Diffuse, "Textures/foreground.jpg");
	}

	one.loadTexture(Diffuse, "Textures/red.png");
	//one.move(0, 10);

	LoadLevel("levels/level1.txt");

	//one.move(0, 0);

}

void Foreground::Update(float dt)
{
	time += dt;

	if (time >= delay)
	{
		time = 0;
		float x = -22.5f;
		if (!piece.empty())
		{
			for (int i = 0; i < 4; i++)
			{
				GameObject* temp = new GameObject();

				if (assetIncrement < 0)
					assetIncrement = assetLength;

				if (piece[assetIncrement] == '1')
				{
					temp->rotate = glm::rotate(assets[0].rotate, 89.55f, glm::vec3(1.0f, 0.0f, 0.0f));
					temp->scale = glm::scale(assets[0].scale, glm::vec3(scale));
					temp->mesh = assets[0].mesh;
					temp->mat = assets[0].mat;
				}
				else if (piece[assetIncrement] == '2')
				{
					temp->rotate = glm::rotate(assets[1].rotate, 89.55f, glm::vec3(1.0f, 0.0f, 0.0f));
					temp->scale = glm::scale(assets[1].scale, glm::vec3(scale));
					temp->mesh = assets[1].mesh;
					temp->mat = assets[1].mat;
				}
				else if (piece[assetIncrement] == '3')
				{
					temp->rotate = glm::rotate(assets[2].rotate, 89.55f, glm::vec3(1.0f, 0.0f, 0.0f));
					temp->scale = glm::scale(assets[2].scale, glm::vec3(scale));
					temp->mesh = assets[2].mesh;
					temp->mat = assets[2].mat;
				}

				temp->move(x, 40, -.7f);
				Pieces.push_back(temp);
				x += offsetX * scale;
				//piece.pop_back();
				assetIncrement--;
			}
		}
	}

	UpdatePieces();
}

void Foreground::UpdatePieces()
{
	for (int i = 0; i < Pieces.size(); i++)
	{
		if (Pieces[i]->location.y <= -40)
			Pieces.erase(Pieces.begin() + i);
		else
			Pieces[i]->move(0, -1 * speed, 0);
	}
}

void Foreground::draw(ShaderProgram & shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, std::vector<Light> pointLights)
{
	for (int i = 0; i < Pieces.size(); i++)
	{
		Pieces[i]->draw(shader, cameraTransform, cameraProjection, pointLights);
	}
}

void Foreground::LoadLevel(std::string fileName)
{
	myFile.open(fileName.c_str());

	std::string line;

	if (myFile)
	{
		while (std::getline(myFile, line))
		{
			for (int i = 0; i < 4; i++)
			{
				piece.push_back(line.c_str()[i]);
			}
		}
		myFile.close();
	}

	assetLength = piece.size() - 1;
	assetIncrement = assetLength;
}
