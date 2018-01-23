#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/functions.hpp>

GameObject bullet;

Game::Game()
{

}

Game::~Game()
{
	delete updateTimer;
	
	phong.unload();
	player.mesh.unload();
	player2.mesh.unload();
	player.projectile.mesh.unload();
	player2.projectile.mesh.unload();
	basicEnemy.mesh.unload();
	basicEnemy.projectile.mesh.unload();
	circleEnemy.mesh.unload();
	circleEnemy.projectile.mesh.unload();
	player.blackBar.mesh.unload();
	player.yellowBar.mesh.unload();
	player2.shield.mesh.unload();

	for (int i = 0; i < players.size(); i++)
	{
		players.erase(players.begin() + i);
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.erase(enemies.begin() + i);
	}
}

//Happens once at the beginning of the game
void Game::initializeGame()
{
	state = title;
	
	std::srand(time(NULL));

	glEnable(GL_DEPTH_TEST);

	Light light1;

	light1.position = glm::vec4(0.0f, 0.0f, 50.0f, 1.0f);
	light1.ogPosition = light1.position;
	light1.ambient = glm::vec3(1.0f);
	light1.diffuse = glm::vec3(30.0f);
	light1.specular = glm::vec3(0.5f);
	light1.specularExponent = 1.0f;
	light1.constantAttenuation = 0.01f;
	light1.linearAttenuation = 0.01f;
	light1.quadraticAttenuation = 0.01f;

	initializeLevel();

	pointLights.push_back(light1);

	//Loading in meshes
	if (!phong.load("shaders/phong.vert", "shaders/phong.frag"))
	{
		std::cout << "Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!textShader.load("shaders/text.vert", "shaders/text.frag"))
	{
		std::cout << "Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!basicPlayer.mesh.loadFromFile("meshes/Player_Ship.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!combinedPlayer.mesh.loadFromFile("meshes/Combined_Ship.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!bullet.mesh.loadFromFile("meshes/bullet1.obj"))
	{
		std::cout << "Projectile model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	player.projectile.mesh = bullet.mesh;

	player2.projectile.mesh = bullet.mesh;

	if (!basicEnemy.mesh.loadFromFile("meshes/Basic_Enemy.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!circleEnemy.mesh.loadFromFile("meshes/Circle_Enemy.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	orbitEnemy.mesh = basicEnemy.mesh;

	orbitEnemy.projectile.mesh = bullet.mesh;
	
	basicEnemy.projectile.mesh = bullet.mesh;
	
	circleEnemy.projectile.mesh = bullet.mesh;

	if (!player.blackBar.mesh.loadFromFile("meshes/bar.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player.yellowBar.mesh.loadFromFile("meshes/bar.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player2.shield.mesh.loadFromFile("meshes/shield.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!playButton.mesh.loadFromFile("meshes/button.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}


	player.mesh = basicPlayer.mesh;
	player2.mesh = basicPlayer.mesh;

	quitButton.mesh = playButton.mesh;
	playButton.setLocation(0, 0);
	quitButton.setLocation(0, -8);

	playButton.scale = glm::scale(playButton.scale, glm::vec3(4.0f, 4.0f, 4.0f));
	quitButton.scale = glm::scale(quitButton.scale, glm::vec3(4.0f, 4.0f, 4.0f));

	player2.blackBar = player.blackBar;
	player2.yellowBar = player.yellowBar;

	cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f, 0.0f, -30.0f));
	cameraProjection = glm::perspective(45.0f, ((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)), 0.1f, 10000.0f);
	cameraOrtho = glm::ortho(-10.f, 10.f, -10.f, 10.f, -30.f, 100.f);

	player.baseMat.loadTexture(Diffuse, "Textures/Player_Ship_B.png");
	player2.baseMat.loadTexture(Diffuse, "Textures/Player_Ship_Y.png");

	basicEnemy.loadTexture(Diffuse, "Textures/Basic_Enemy.png");
	circleEnemy.loadTexture(Diffuse, "Textures/Circle_Enemy.png");

	red.loadTexture(Diffuse, "Textures/red.png");
	blue.loadTexture(Diffuse, "Textures/blue.png");
	yellow.loadTexture(Diffuse, "Textures/yellow.png");

	play.loadTexture(Diffuse, "Textures/Play.png");
	play_un.loadTexture(Diffuse, "Textures/Play_Un.png");
	quit.loadTexture(Diffuse, "Textures/Quit.png");
	quit_un.loadTexture(Diffuse, "Textures/Quit_Un.png");
	combinedPlayer.loadTexture(Diffuse, "Textures/Green.png");

	playButton.mat = play;
	quitButton.mat = quit_un;

	player.mat = player.baseMat;
	player.projectile.mat = blue;

	player2.mat = player2.baseMat;
	player2.projectile.mat = yellow;

	player.setNum(0);
	player2.setNum(1);
		
	orbitEnemy.mat = basicEnemy.mat;

	basicEnemy.projectile.mat = red;
	circleEnemy.projectile.mat = red;
	orbitEnemy.projectile.mat = red;

	player.blackBar.loadTexture(Diffuse, "Textures/Black.png");
	player.yellowBar.mat = yellow;

	player.yellowBar.rotate = glm::rotate(player.yellowBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.blackBar.rotate = glm::rotate(player.blackBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.yellowBar.scale = glm::scale(player.yellowBar.scale, glm::vec3(.3f));
	player.blackBar.scale = glm::scale(player.blackBar.scale, glm::vec3(.3f));
	player.yellowBar.move(0, -8.0f);
	player.blackBar.move(0, -8.0f);

	player2.shield.loadTexture(Diffuse, "Textures/cyan.png");
	
	player.move(-10.0f, -5.0f);
	player2.move(10.0f, -5.0f);
	players.push_back(&player);
	players.push_back(&player2);
															 
	playButton.rotate = glm::rotate(playButton.ogRotate, acos(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	quitButton.rotate = glm::rotate(playButton.ogRotate, acos(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	playButton.move(0, 0);
	quitButton.move(0, 0);

	background.Initialize();

	text.Intialize("Square.ttf");
}

//Happens once per frame, used to update state of the game
void Game::update()
{
	if (state == title)
	{
		draw();

		player.controller.DownloadPackets(2);
		player.controller.GetSticks(player.playerNum, player.lStick, player.rStick);

		if(selected == none)
		{ 
			playButton.mat = play_un;
			quitButton.mat = quit_un;
		}

		bool tilted = std::abs(player.lStick.yAxis) > 0.25f ? true : false;
		if (tilted)
		{
			if (((selected == _play) || (selected == none)) && (player.lStick.yAxis < 0))
			{
				selected = _quit;
				playButton.mat = play_un;
				quitButton.mat = quit;
			}

			if (((selected == _quit) || (selected == none)) && (player.lStick.yAxis > 0))
			{
				selected = _play;
				playButton.mat = play;
				quitButton.mat = quit_un;
			}
		}

		if (player.controller.GetButton(player.playerNum, XBox::A))
		{
			if (selected == _play)
			{
				updateTimer = new Timer();
				state = main;
				background.restart();
			}

			else if(selected == _quit)
				exit(0);
		}

	}

	if (state == main)
	{
		
		//std::cout << "Main" << std::endl;
		//Update timer so we have correct delta time since last update
		updateTimer->tick();
		delay += updateTimer->getElapsedTimeS();
		

		background.update();

		if (player.isTransformed && player2.isTransformed)
		{
			player.mesh = combinedPlayer.mesh;
			player2.mesh = combinedPlayer.mesh;

			player.mat = combinedPlayer.mat;

			player2.mat = combinedPlayer.mat;
		}

		if (!player.isTransformed && player.progress <= 1.0f)
		{
			player.mesh = basicPlayer.mesh;
			player2.mesh = basicPlayer.mesh;

			player.mat = player.baseMat;
			player.projectile.mat = blue;

			player2.mat = player2.baseMat;
			player2.projectile.mat = yellow;
		}

		for (int i = 0; i < players.size(); i++)
		{
			if (players[i]->isAlive())
				players[i]->update(&enemies, players[(i + 1) % 2]);

			else
			{
				players[i]->spawnTime += updateTimer->getElapsedTimeS();

				if ((players[i]->spawnTime >= 2.0f) && (players[i]->numLives > 0))
				{
					players[i]->setLocation(0.0f, 0.0f);
					players[i]->playerState = player.state::alive;
					players[i]->spawnTime = 0.0f;
					players[i]->numLives--;
				}
			}
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i]->update(players, &enemyProjectiles);

			if (enemies[i]->location.y <= -20)
			{
				enemies.erase(enemies.begin() + i);
				break;
			}
		}	

		updateEnemyProjectiles();

		if ((player.numLives == 0) && (player2.numLives == 0))
			state = gameOver;

		if ((currentEnemy < level1.size()) && (level1[currentEnemy]->spawnTime <= delay))
		{
			if (level1[currentEnemy]->type == basic)
			{
				BasicEnemy* temp = new BasicEnemy();
				temp->mesh = basicEnemy.mesh;
				temp->mat = basicEnemy.mat;
				temp->projectile.mesh = basicEnemy.projectile.mesh;
				temp->projectile.mat = basicEnemy.projectile.mat;

				temp->setLocation(level1[currentEnemy]->location.x, level1[currentEnemy]->location.y);

				enemies.push_back(temp);
			}

			if (level1[currentEnemy]->type == circle)
			{
				CircleEnemy* temp = new CircleEnemy();
				temp->mesh = circleEnemy.mesh;
				temp->mat = circleEnemy.mat;
				temp->projectile.mesh = circleEnemy.projectile.mesh;
				temp->projectile.mat = circleEnemy.projectile.mat;

				temp->setLocation(level1[currentEnemy]->location.x, level1[currentEnemy]->location.y);

				enemies.push_back(temp);
			}

			if (level1[currentEnemy]->type == orbit)
			{
				OrbitEnemy* temp = new OrbitEnemy();
				temp->mesh = orbitEnemy.mesh;
				temp->mat = orbitEnemy.mat;
				temp->projectile.mesh = orbitEnemy.projectile.mesh;
				temp->projectile.mat = orbitEnemy.projectile.mat;

				temp->setLocation(level1[currentEnemy]->location.x, level1[currentEnemy]->location.y);

				enemies.push_back(temp);
			}

			currentEnemy++;
		}

		if ((currentEnemy == level1.size()) && (enemies.size() == 0))
		{
			state = win;
		}
	}

	if (state == win)
	{
		background.victory();

		draw();

		for (int i = 0; i < players.size(); i++)
		{
			players[i]->controller.DownloadPackets(2);

			if (players[i]->controller.GetButton(i, XBox::A))
			{
				//state = title;
				//background.mainMenu();
				//empty = false;
				exit(0);
			}
		}
	}

	if (state == gameOver)
	{
		if (!empty)
		{
			emptyGame();

			background.gameOver();
		}


		draw();

		for (int i = 0; i < players.size(); i++)
		{
			players[i]->controller.DownloadPackets(2);

			if (players[i]->controller.GetButton(i, XBox::A))
			{
				//state = title;
				//background.mainMenu();
				//empty = false;
				exit(0);
			}
		}
	}
}

void Game::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (state == main)
	{
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player.score), -9.5, -8, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Lives: " + std::to_string(player.numLives), -9.5, -7, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player2.score), 7, -8, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Lives: " + std::to_string(player2.numLives), 7, -7, .01f, glm::vec3(1, 1, 0));

		if (player.progress == player.transformMax)
			text.RenderText(textShader, cameraOrtho, "Press LB to transform", -2.5f, -7, .01f, glm::vec3(1));
	}

	if (state == win)
	{
		float accuracy1 = player.getAccuracy();
		float accuracy2 = player2.getAccuracy();
		text.RenderText(textShader, cameraOrtho, "Player 1 Score: ", -9.5, -6, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player.score), -9.5, -7, .01f, glm::vec3(0, 0, 1));
		//	text.RenderText(textShader, cameraOrtho, "Acc: " + std::to_string(accuracy1) + "%", -9.5, -8, .01f, glm::vec3(0, 0, 1));

		text.RenderText(textShader, cameraOrtho, "Player 2 Score: ", 5.5f, -6, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player2.score), 5.5f, -7, .01f, glm::vec3(1, 1, 0));
		//text.RenderText(textShader, cameraOrtho, "Acc: " + std::to_string(accuracy2) + "%", 5.5f, -8, .01f, glm::vec3(1, 1, 0));
	}

	if (state == gameOver)
	{
		float accuracy1 = player.getAccuracy();
		float accuracy2 = player2.getAccuracy();
		text.RenderText(textShader, cameraOrtho, "Player 1 Score: ", -9.5, -6, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player.score), -9.5, -7, .01f, glm::vec3(0, 0, 1));
	//	text.RenderText(textShader, cameraOrtho, "Acc: " + std::to_string(accuracy1) + "%", -9.5, -8, .01f, glm::vec3(0, 0, 1));

		text.RenderText(textShader, cameraOrtho, "Player 2 Score: ", 5.5f, -6, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player2.score), 5.5f, -7, .01f, glm::vec3(1, 1, 0));
		//text.RenderText(textShader, cameraOrtho, "Acc: " + std::to_string(accuracy2) + "%", 5.5f, -8, .01f, glm::vec3(1, 1, 0));
	}

	phong.bind();

	if (state == title)
	{
		playButton.draw(phong, cameraTransform, cameraProjection, pointLights);
		quitButton.draw(phong, cameraTransform, cameraProjection, pointLights);
		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
	}

	else if (state == gameOver)
	{
		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
	}

	else if (state == win)
	{
		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
	}

	else
	{
		if (player.isTransformed && player2.isTransformed)
			player2.shield.draw(phong, cameraTransform, cameraProjection, pointLights);

		//Iterate through vector of enemies and draw each one
		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i]->draw(phong, cameraTransform, cameraProjection, pointLights);
		}

		for (int i = 0; i < enemyProjectiles.size(); i++)
		{
			enemyProjectiles[i]->draw(phong, cameraTransform, cameraProjection, pointLights);
		}

		for (int i = 0; i < players.size(); i++)
		{
			if (players[i]->isAlive())
			{
				players[i]->draw(phong, cameraTransform, cameraProjection, pointLights);
			}
			for (int j = 0; j < players[i]->getProjectiles().size(); j++)
			{
				players[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
			}
		}

		player.blackBar.draw(phong, cameraTransform, cameraOrtho, pointLights);
		player.yellowBar.draw(phong, cameraTransform, cameraOrtho, pointLights);

		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
	}

	phong.unbind();

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 27:
	case 'q':
		exit(1);
		break;

	case 't':
		std::cout << "Total elapsed time: " << updateTimer->getCurrentTime() << std::endl;
		break;

	case 'r':
	case 'R':
		if (state == gameOver)
			initializeGame();

		break;

	case 'l':
	case 'L':
		shouldLightsSpin = !shouldLightsSpin;
		break;

	default:
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	default:
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			//handle left click
			break;

		case GLUT_RIGHT_BUTTON:
			//handle right click
			break;

		case GLUT_MIDDLE_BUTTON:
			//handle middle button
			break;

		default:
			break;
		}
	}
}

void Game::mouseMoved(int x, int y)
{

}

void Game::updateEnemyProjectiles()
{
	for (int i = 0; i < enemyProjectiles.size(); i++)
	{
		enemyProjectiles[i]->move(enemyProjectiles[i]->getVelocity().x, enemyProjectiles[i]->getVelocity().y);

		if (players[0]->getTransform() && players[1]->getTransform())
		{
			if (enemyProjectiles[i]->collide(players[1]->shield))
			{
				enemyProjectiles.erase(enemyProjectiles.begin() + i);
				break;
			}
		}

		GameObject temp;
		temp.location = enemyProjectiles[i]->location;
		temp.radius = enemyProjectiles[i]->radius;

		if ((players[0]->collide(temp)) && (players[0]->playerState == player.state::alive)) 
		{
			players[0]->setDead();
			enemyProjectiles.erase(enemyProjectiles.begin() + i);

			for (int j = 0; j < players[0]->projectiles.size(); i++)
			{
				players[0]->projectiles.erase(players[0]->projectiles.begin() + j);
			}

			break;
		}
		
		
		if (players[1]->collide(temp) && (players[1]->playerState == player.state::alive))
		{
			players[1]->setDead();
			enemyProjectiles.erase(enemyProjectiles.begin() + i);

			for(int j = 0; j < players[1]->projectiles.size(); i++)
			{
				players[1]->projectiles.erase(players[1]->projectiles.begin() + j);
			}
			break;
		}


		if (enemyProjectiles[i]->isOffscreen())
		{
			enemyProjectiles.erase(enemyProjectiles.begin() + i);
			break;
		}
	}
}

void Game::emptyGame()
{
	updateTimer = new Timer();
	delay = 0.0f;
	player.numLives = 3;
	player2.numLives = 3;

	for (int i = 0; i < players.size(); i++)
	{
		for (int j = 0; j < players[i]->projectiles.size(); j++)
		{
			delete players[i]->projectiles[j];
		}

		players[i]->projectiles.clear();
	}

	for (int i = 0; i < enemies.size(); i++)
	{

		delete enemies[i];
	}

	enemies.clear();

	for (int i = 0; i < enemyProjectiles.size(); i++)
	{
		delete enemyProjectiles[i];
	}

	enemyProjectiles.clear();
	background.gameOver();

	player.playerState = player.state::alive;
	player2.playerState = player.state::alive;

	basicEnemy.mat = red;
	basicEnemy.projectile.mat = red;
	circleEnemy.mat = red;
	circleEnemy.projectile.mat = red;

	selected = none;

	background.gameOver();

	empty = true;
}

void Game::initializeLevel()
{

	level1.push_back(new enemyNode(2, glm::vec2(10, 20), basic));

	level1.push_back(new enemyNode(8, glm::vec2(10, 20), basic));
	level1.push_back(new enemyNode(8, glm::vec2(-10, 20), basic));

	level1.push_back(new enemyNode(15, glm::vec2(-10, 20), basic));
	level1.push_back(new enemyNode(15, glm::vec2(0, 20), basic));
	level1.push_back(new enemyNode(15, glm::vec2(10, 20), basic));
	
	level1.push_back(new enemyNode(25, glm::vec2(0, 20), circle));
	
	level1.push_back(new enemyNode(35, glm::vec2(-10, 20), basic));
	level1.push_back(new enemyNode(35, glm::vec2(0, 20), circle));
	level1.push_back(new enemyNode(35, glm::vec2(10, 20), basic));
	
	level1.push_back(new enemyNode(45, glm::vec2(-19, 20), basic));
	level1.push_back(new enemyNode(45, glm::vec2(-10, 20), circle));
	level1.push_back(new enemyNode(45, glm::vec2(0, 20), basic));
	level1.push_back(new enemyNode(45, glm::vec2(10, 20), circle));
	level1.push_back(new enemyNode(45, glm::vec2(19, 20), basic));
	
	level1.push_back(new enemyNode(55, glm::vec2(-10, 20), basic));
	level1.push_back(new enemyNode(55, glm::vec2(0, 20), orbit));
	level1.push_back(new enemyNode(55, glm::vec2(10, 20), basic));

	level1.push_back(new enemyNode(62, glm::vec2(-10, 20), orbit));
	level1.push_back(new enemyNode(62, glm::vec2(0, 20), basic));
	level1.push_back(new enemyNode(62, glm::vec2(10, 20), orbit));

	level1.push_back(new enemyNode(70, glm::vec2(0, 20), orbit));
	level1.push_back(new enemyNode(70, glm::vec2(-30, 0), orbit));
	level1.push_back(new enemyNode(70, glm::vec2(30, 0), orbit));

	level1.push_back(new enemyNode(75, glm::vec2(0, 20), circle));
	level1.push_back(new enemyNode(75, glm::vec2(-30, 0), orbit));
	level1.push_back(new enemyNode(75, glm::vec2(30, 0), orbit));

	level1.push_back(new enemyNode(80, glm::vec2(-10, 20), circle));
	level1.push_back(new enemyNode(80, glm::vec2(10, 20), circle));
	level1.push_back(new enemyNode(80, glm::vec2(-30, 0), orbit));
	level1.push_back(new enemyNode(80, glm::vec2(30, 0), orbit));

	level1.push_back(new enemyNode(90, glm::vec2(-10, 20), circle));
	level1.push_back(new enemyNode(90, glm::vec2(10, 20), circle));
	level1.push_back(new enemyNode(90, glm::vec2(0, 20), circle));
	level1.push_back(new enemyNode(90, glm::vec2(-30, 0), orbit));
	level1.push_back(new enemyNode(90, glm::vec2(30, 0), orbit));
	level1.push_back(new enemyNode(90, glm::vec2(10, 20), basic));
	level1.push_back(new enemyNode(90, glm::vec2(-10, 20), basic));
	
}