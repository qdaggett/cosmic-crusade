#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/functions.hpp>
#include "SoundEngine.h"

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
	enemyManager.Unload();

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

	player.mesh = basicPlayer.mesh;
	player2.mesh = basicPlayer.mesh;

	if (!bullet.mesh.loadFromFile("meshes/bullet1.obj"))
	{
		std::cout << "Projectile model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	player.projectile.mesh = bullet.mesh;

	player2.projectile.mesh = bullet.mesh;

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

	quitButton.mesh = playButton.mesh;
	playButton.setLocation(0, 0);
	quitButton.setLocation(0, -8);

	playButton.scale = glm::scale(playButton.scale, glm::vec3(4.0f, 4.0f, 4.0f));
	quitButton.scale = glm::scale(quitButton.scale, glm::vec3(4.0f, 4.0f, 4.0f));

	player2.blackBar = player.blackBar;
	player2.yellowBar = player.yellowBar;

	cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f, 0.0f, -30.0f));
	cameraProjection = glm::perspective(45.0f, ((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)), 0.1f, 10000.0f);
	cameraOrtho = glm::ortho(-10.f, 10.f, -10.f, 10.f, -30.f, 1000.f);

	player.baseMat.loadTexture(Diffuse, "Textures/Player_Ship_B.png");
	player2.baseMat.loadTexture(Diffuse, "Textures/Player_Ship_Y.png");

	basicEnemy.loadTexture(Diffuse, "Textures/Basic_Enemy.png");

	red.loadTexture(Diffuse, "Textures/red.png");
	blue.loadTexture(Diffuse, "Textures/blue.png");
	yellow.loadTexture(Diffuse, "Textures/yellow.png");
	green.loadTexture(Diffuse, "Textures/green.png");

	play.loadTexture(Diffuse, "Textures/Play.png");
	play_un.loadTexture(Diffuse, "Textures/Play_Un.png");
	quit.loadTexture(Diffuse, "Textures/Quit.png");
	quit_un.loadTexture(Diffuse, "Textures/Quit_Un.png");

	playButton.mat = play;
	quitButton.mat = quit_un;

	player.mat = player.baseMat;
	player.projectile.mat = blue;

	player2.mat = player2.baseMat;
	player2.projectile.mat = yellow;

	player.setNum(0);
	player2.setNum(1);

	player.blackBar.loadTexture(Diffuse, "Textures/Black.png");
	player.yellowBar.mat = yellow;

	player.yellowBar.rotate = glm::rotate(player.yellowBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.blackBar.rotate = glm::rotate(player.blackBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.yellowBar.scale = glm::scale(player.yellowBar.scale, glm::vec3(.3f));
	player.blackBar.scale = glm::scale(player.blackBar.scale, glm::vec3(.3f));
	player.yellowBar.move(0, -8.0f);
	player.blackBar.move(0, -8.0f);

	player2.shield.loadTexture(Diffuse, "Textures/cyan.png");
	
	player.move(10.0f, -5.0f);
	player2.move(-10.0f, -5.0f);
	players.push_back(&player);
	players.push_back(&player2);
															 
	playButton.rotate = glm::rotate(playButton.ogRotate, acos(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	quitButton.rotate = glm::rotate(playButton.ogRotate, acos(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	playButton.move(0, 0);
	quitButton.move(0, 0);

	background.Initialize();

	std::cout << player.radius << std::endl;

	text.Intialize("Square.ttf");

	foreground.Intialize();
	enemyManager.Intialize(players);

	// Initializing sound engine
	se.Init();

	// Loading in sound files
	result = se.system->createSound("sounds/failsound.wav", FMOD_3D, 0, &failsound);
	FModErrorCheck(result);
	result = failsound->set3DMinMaxDistance(0.5f, 300.0f);
	FModErrorCheck(result);

	// Setting the failsound to not repeat
	result = failsound->setMode(FMOD_LOOP_OFF);
}

//Happens once per frame, used to update state of the game
void Game::update()
{
	// FMOD update function
	result = se.system->update();
	FModErrorCheck(result);

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
		//foreground.Update(updateTimer->getElapsedTimeS());
		enemyManager.Update(updateTimer->getElapsedTimeS());

		if (player.isTransformed && player2.isTransformed)
		{
			player.mesh = combinedPlayer.mesh;
			player2.mesh = combinedPlayer.mesh;

			player.mat = green;
			player.projectile.mat = green;

			player2.mat = green;
			player2.projectile.mat = green;
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
				players[i]->update(&enemyManager.enemyList, players[(i + 1) % 2]);

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

			if (players[i]->location.x >= 29)
				players[i]->setLocation(29, players[i]->location.y);

			if (players[i]->location.x <= -29)
				players[i]->setLocation(-29, players[i]->location.y);

			if (players[i]->location.y >= 16)
				players[i]->setLocation(players[i]->location.x, 16);

			if (players[i]->location.y <= -16)
				players[i]->setLocation(players[i]->location.x, -16);
		}


		if ((player.numLives == 0) && (player2.numLives == 0) || currentEnemy == level1.size() - 1 && enemies.empty())
			state = gameOver;


		//Player Boundary
		
	}

	if (state == gameOver)
	{
		if (!empty)
		{
			emptyGame();

			background.gameOver();

			FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

			result = se.system->playSound(failsound, 0, true, &channel);
			FModErrorCheck(result);
			result = channel->set3DAttributes(&pos, &vel);
			FModErrorCheck(result);
			result = channel->setPaused(false);
			FModErrorCheck(result);
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
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(players[0]->score), -9.5, -8, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Lives: " + std::to_string(players[0]->numLives), -9.5, -7, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(players[1]->score), 7, -8, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Lives: " + std::to_string(players[1]->numLives), 7, -7, .01f, glm::vec3(1, 1, 0));

		if (delay < 5)
		{
			text.RenderText(textShader, cameraOrtho, "LeftStick - Move", -2, 0.5f, 0.01f, glm::vec3(1));
			text.RenderText(textShader, cameraOrtho, "RightStick - Aim/Shoot", -2.5f, -.5f, 0.01f, glm::vec3(1));
		}

		if (player.progress == player.transformMax)
			text.RenderText(textShader, cameraOrtho, "Press LB to transform", -2.5f, -7, .01f, glm::vec3(1));
	}

	if (state == gameOver)
	{
		text.RenderText(textShader, cameraOrtho, "Player 1 Score: " , -9.5, -6, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(players[0]->score), -9.5, -7, .01f, glm::vec3(0, 0, 1));
		//float num = (players[0]->hits / players[0]->totalShots) * 100;
		//text.RenderText(textShader, cameraOrtho, "Acc: " + std::to_string(num) + "%", -9.5, -8, .01f, glm::vec3(0, 0, 1));

		text.RenderText(textShader, cameraOrtho, "Player 2 Score: ", 5.5f, -6, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(players[1]->score), 5.5f, -7, .01f, glm::vec3(1, 1, 0));
	//	text.RenderText(textShader, cameraOrtho, "Acc: " + std::to_string(players[1]->getAccuracy()) + "%", 5.5f, -8, .01f, glm::vec3(1, 1, 0));
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

	else
	{
		if (player.isTransformed && player2.isTransformed)
			player2.shield.draw(phong, cameraTransform, cameraProjection, pointLights);

		enemyManager.Draw(phong, cameraTransform, cameraProjection, pointLights);

		for (int i = 0; i < players.size(); i++)
		{
			if (players[i]->isAlive())
			{
				players[i]->draw(phong, cameraTransform, cameraProjection, pointLights);

				for (int j = 0; j < players[i]->getProjectiles().size(); j++)
				{
					players[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
				}
			}
		}

		player.blackBar.draw(phong, cameraTransform, cameraOrtho, pointLights);
		player.yellowBar.draw(phong, cameraTransform, cameraOrtho, pointLights);

		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
		foreground.draw(phong, cameraTransform, cameraProjection, pointLights);
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
	case 'a':
		debugSelect = true;
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
			break;
		}
		
		
		if (players[1]->collide(temp) && (players[1]->playerState == player.state::alive))
		{
			players[1]->setDead();
			enemyProjectiles.erase(enemyProjectiles.begin() + i);
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
