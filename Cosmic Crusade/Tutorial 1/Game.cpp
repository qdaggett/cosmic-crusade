#pragma once
#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <glm/gtc/functions.hpp>
#include "SoundEngine.h"
#include "SoundEffect.h"
#include <math.h>
#include <Windows.h>
#include <minwindef.h>


GameObject bullet;

glm::mat4 newPosition = glm::translate(newPosition, glm::vec3(0, 0, -35));
#define min(a,b)            (((a) < (b)) ? (a) : (b));

float Random(float a, float b)
{
	return (float(rand()) / float(RAND_MAX)) * abs(b - a) + min(a, b);
}


glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float dt)
{
	glm::vec3 pos = glm::vec3(((1 - dt) * x.x) + (dt * y.x), ((1 - dt) * x.y) + (dt * y.y), ((1 - dt) * x.z) + (dt * y.z));
	//std::cout << pos.x << ", " << pos.y << ", " << pos.z << endl;
	return pos;
}

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
	player.blackBar.mesh.unload();
	player.yellowBar.mesh.unload();
	player2.shield.mesh.unload();
	enemyManager.Unload();

	for (int i = 0; i < players.size(); i++)
		players.erase(players.begin() + i);

	for (int i = 0; i < fbos.size(); i++)
		fbos.erase(fbos.begin() + i);
}


//Happens once at the beginning of the game
void Game::initializeGame()
{
	state = score;

	//	state = main; updateTimer = new Timer();

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



	if (!particleShader.load("shaders/passThru.vert", "shaders/particles.geom", "shaders/unlitTextureP.frag"))
	{
		std::cout << "Particle Shader failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!unlitShader.load("shaders/unlit.vert", "shaders/unlit.frag"))
	{
		std::cout << "Unlit Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!brightPass.load("shaders/unlit.vert", "shaders/bright.frag"))
	{
		std::cout << "Brightpass Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!blurShader.load("shaders/unlit.vert", "shaders/blur.frag"))
	{
		std::cout << "Blur Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!bloomShader.load("shaders/unlit.vert", "shaders/bloom.frag"))
	{
		std::cout << "Bloom Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!orComposite.load("shaders/unlit.vert", "shaders/orComp.frag"))
	{
		std::cout << "orComp shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!textShader.load("shaders/text.vert", "shaders/text.frag"))
	{
		std::cout << "Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	//	if (!rimShader.load("shaders/unlit.vert", "shaders/rimLight.frag"))
	//	{
	//		std::cout << "Rim Shaders failed to initialize." << std::endl;
	//		system("pause");
	//		exit(0);
	//	}


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

	if (!player.turret.mesh.loadFromFile("meshes/turret.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	player2.turret.mesh = player.turret.mesh;

	player.mesh = basicPlayer.mesh;

	player2.mesh = basicPlayer.mesh;

	//ammoPowerUp.mesh = powerup.mesh;

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
		std::cout << "Bar model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	player.yellowBar.mesh = player.blackBar.mesh;

	if (!player2.shield.mesh.loadFromFile("meshes/shield.obj"))
	{
		std::cout << "Shield model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!playButton.mesh.loadFromFile("meshes/button.obj"))
	{
		std::cout << "Button model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player.reticle.mesh.loadFromFile("meshes/reticle.obj"))
	{
		std::cout << "reticle model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	player2.reticle.mesh = player.reticle.mesh;

	//if (!fullscreenQuad.mesh.loadFromFile("meshes/plane.obj"))
	//{
	//	std::cout << "Fullscreen Quad model failed to load." << std::endl;
	//	system("pause");
	//	exit(0);
	//}

	quitButton.mesh = playButton.mesh;
	playButton.setLocation(0, 0);
	quitButton.setLocation(0, -8);

	playButton.scale = glm::scale(playButton.scale, glm::vec3(4.0f, 4.0f, 4.0f));
	quitButton.scale = glm::scale(quitButton.scale, glm::vec3(4.0f, 4.0f, 4.0f));

	player2.blackBar = player.blackBar;
	player2.yellowBar = player.yellowBar;

	cameraTranslation = glm::vec3(0.0f, 0.0f, -30.0f);
	cameraTransform = glm::translate(cameraTransform, cameraTranslation);
	cameraProjection = glm::perspective(45.0f, ((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)), 0.1f, 10000.0f);
	cameraOrtho = glm::ortho(-10.f, 10.f, -10.f, 10.f, -30.f, 1000.f);

	player.baseMat.loadTexture(Diffuse, "Textures/Player_Ship_B.png");
	player.shield.mesh = player2.shield.mesh;

	//player.loadTexture(Diffuse, "Textures/Shield.png");
	player2.shield.loadTexture(Diffuse, "Textures/Shield.png");

	player2.baseMat.loadTexture(Diffuse, "Textures/Player_Ship_Y.png");


	red.loadTexture(Diffuse, "Textures/red.png");
	blue.loadTexture(Diffuse, "Textures/blue.png");
	yellow.loadTexture(Diffuse, "Textures/yellow.png");
	green.loadTexture(Diffuse, "Textures/green.png");

	play.loadTexture(Diffuse, "Textures/Play.png");
	play_un.loadTexture(Diffuse, "Textures/Play_Un.png");
	quit.loadTexture(Diffuse, "Textures/Quit.png");
	quit_un.loadTexture(Diffuse, "Textures/Quit_Un.png");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	phong.bind();
	background.Initialize();
	background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
	phong.unbind();
	glutSwapBuffers();

	playButton.mat = play;
	quitButton.mat = quit_un;

	player.mat = player.baseMat;
	player.projectile.mat = blue;
	player.turret.mat.loadTexture(Diffuse, "Textures/Turret_blue_diffuse.png");

	player2.mat = player2.baseMat;
	player2.projectile.mat = yellow;
	player2.turret.mat.loadTexture(Diffuse, "Textures/Turret_yellow_diffuse.png");


	player.setNum(0);
	player2.setNum(1);

	player.blackBar.loadTexture(Diffuse, "Textures/Black.png");
	player.yellowBar.mat = yellow;

	player.yellowBar.rotate = glm::rotate(player.yellowBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.blackBar.rotate = glm::rotate(player.blackBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.yellowBar.scale = glm::scale(player.yellowBar.scale, glm::vec3(.3f));
	player.blackBar.scale = glm::scale(player.blackBar.scale, glm::vec3(.3f));
	player.yellowBar.ogScale = player.yellowBar.scale;
	player.blackBar.ogScale = player.blackBar.scale;
	//player2.yellowBar.scale = glm::scale(player.yellowBar.scale, glm::vec3(0.5, 5.0f, 1.0f));
	//player2.blackBar.scale = glm::scale(player.blackBar.scale, glm::vec3(0.5f, 5.0f, 1.0f));
	player2.yellowBar = player.yellowBar;
	player2.blackBar = player.blackBar;

	player.yellowBar.move(0, -8.0f);
	player.blackBar.move(0, -8.0f);

	player.reticle.mat = blue;
	player2.reticle.mat = yellow;

	player2.shield.collider = new Collider(Collider::SPHERE, 0.5f);

	player.move(-10.0f, -5.0f);
	player2.move(10.0f, -5.0f);
	players.push_back(&player);
	players.push_back(&player2);

	playButton.rotate = glm::rotate(playButton.ogRotate, acos(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	quitButton.rotate = glm::rotate(playButton.ogRotate, acos(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	playButton.move(0, 0);
	quitButton.move(0, 0);

	std::cout << player.radius << std::endl;

	text.Intialize("Square.ttf");

	foreground.Intialize();
	enemyManager.Intialize(players, &emitters);

	ammoPowerUp.initializePowerUp(&emitters);
	ammoPowerUp.mat = green;

	fuelPowerUp.initializePowerUp(&emitters);
	fuelPowerUp.mat = blue;

	timePowerUp.initializePowerUp(&emitters);
	timePowerUp.loadTexture(Diffuse, "Textures/Hourglass.png");

	gameSounds.initializeSounds();

	float blurAmount = 16.0f;

	def.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN), 2, true);
	bright.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN), 2, true);
	blur_a.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN) / blurAmount, (float)GetSystemMetrics(SM_CYSCREEN) / blurAmount, 2, true);
	blur_b.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN) / blurAmount, (float)GetSystemMetrics(SM_CYSCREEN) / blurAmount, 2, true);
	lowRes.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN) / blurAmount, (float)GetSystemMetrics(SM_CYSCREEN) / blurAmount, 2, true);
	toBloom.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN), 2, true);
	text_fbo.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN), 2, true);
	rimBuffer.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN), 1, true);
	background_fbo.createFrameBuffer((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN), 2, true);
	fullscreenQuad.create();
	LoadScores();
	fbos.push_back(&def);
	fbos.push_back(&bright);
	fbos.push_back(&blur_a);
	fbos.push_back(&blur_b);
	fbos.push_back(&lowRes);
	fbos.push_back(&toBloom);
	fbos.push_back(&text_fbo);
	fbos.push_back(&background_fbo);

	background.mainMenu();
}

//Happens once per frame, used to update state of the game
void Game::update()
{
	// FMOD update function
	gameSounds.updateSounds();
	//camera.update();

	// This is what makes the controllers function
	player.controller.DownloadPackets(2);
	player.controller.GetSticks(player.playerNum, player.lStick, player.rStick);

	for (int i = 0; i < emitters.size(); i++)
	{
		if (emitters[i]->playing)
			emitters[i]->update(deltaTime);

		else
			emitters.erase(emitters.begin() + i);
	}


	if (state == title)
	{
		if (selected == none)
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
				state = monologue;
			}

			else if (selected == _quit)
				exit(0);
		}
	}

	// Monologue state, this is where my shitty voice acting plays
	if (state == monologue)
	{
		// Making sure the sound plays only once
		if (gameSounds.introHasPlayed == false)
		{
			gameSounds.playSound(gameSounds.monologue, &gameSounds.monologueChannel);

			gameSounds.introHasPlayed = true;
		}

		// Pressing B skips the monologue
		if (player.controller.GetButton(player.playerNum, XBox::B))
		{
			state = main;
			gameSounds.monologueChannel->stop();
		}

		bool isPlaying;
		gameSounds.monologueChannel->isPlaying(&isPlaying);

		if (isPlaying == false)
		{
			updateTimer = new Timer();
			background.updateTimer = new Timer();
			background.restart();
			state = main;
		}

	}

	if (state == main)
	{
		//emitter.play();
		//Update timer so we have correct delta time since last update
		updateTimer->tick();
		delay += updateTimer->getElapsedTimeS();

		float deltaTime = updateTimer->getTotalTime();

		// Updates the starry space background
		background.update();

		// Updates the enemy manager
		enemyManager.Update(updateTimer->getElapsedTimeS());

		if (enemyManager.bossSpawn)
		{
			cameraFloat += updateTimer->getElapsedTimeS();;
			//if (cameraFloat < 1)
			//	cameraTransform = glm::lerp<glm::mat4, float>(cameraTransform, newPosition, cameraFloat);
		}

		// Updates the ammo powerup
		ammoPowerUp.updatePowerUp(players, updateTimer->getElapsedTimeS());

		// Updates the fuel powerup
		fuelPowerUp.updatePowerUp(players, updateTimer->getElapsedTimeS());

		// Updates the time powerup
		timePowerUp.updatePowerUp(players, updateTimer->getElapsedTimeS());

		pauseTime += updateTimer->getElapsedTimeS();
		empty = false;

		if (Player::progress > Player::transformMax)
			Player::progress = Player::transformMax;

		// Game music
		if (gameSounds.hasPlayed == false) {
			gameSounds.playSound(gameSounds.music, &gameSounds.musicChannel);

			gameSounds.hasPlayed = true;
		}

		// This handles the transformation of the players into the combined ship
		if (player.isTransformed && player2.isTransformed)
		{
			player.mesh = combinedPlayer.mesh;
			player2.mesh = combinedPlayer.mesh;

			player.mat = green;
			player.projectile.mat = green;

			player2.mat = green;
			player2.projectile.mat = green;
		}

		// Plays shooting sound when player fires a bullet
		if (player.hasShot == true)
		{
			gameSounds.playSound(gameSounds.shoot, &gameSounds.playerShootChannel);
			player.hasShot = false;
		}

		if (player2.hasShot == true)
		{
			gameSounds.playSound(gameSounds.shoot, &gameSounds.playerShootChannel);
			player2.hasShot = false;
		}

		// For the shotgun sound
		if (player.hasShotShotgun == true)
		{
			gameSounds.playSound(gameSounds.shoot2, &gameSounds.shotgunShootChannel);
			player.hasShotShotgun = false;
		}

		if (player2.hasShotShotgun == true)
		{
			gameSounds.playSound(gameSounds.shoot2, &gameSounds.shotgunShootChannel);
			player2.hasShotShotgun = false;
		}

		// Plays hit sound when player hits an enemy ship
		if (player.hasHit == true)
		{
			float randomizer = Random(1, 4);
			cout << randomizer << endl;
			if (randomizer > 1 && randomizer < 1.99)
				ammoPowerUp.spawnPowerUp(player.tempEnemy);
			else if (randomizer > 2 && randomizer < 2.99)
				fuelPowerUp.spawnPowerUp(player.tempEnemy);
			else if (randomizer > 3 && randomizer < 3.99)
				timePowerUp.spawnPowerUp(player.tempEnemy);
			else
				cout << "Failed to spawn in powerup" << endl;

			gameSounds.playSound(gameSounds.enemyHit, &gameSounds.enemyHitChannel);
			player.hasHit = false;
		}

		if (player2.hasHit == true)
		{
			float randomizer = Random(1, 4);
			cout << randomizer << endl;
			if (randomizer > 1 && randomizer < 1.99)
				ammoPowerUp.spawnPowerUp(player.tempEnemy);
			else if (randomizer > 2 && randomizer < 2.99)
				fuelPowerUp.spawnPowerUp(player.tempEnemy);
			else if (randomizer > 3 && randomizer < 3.99)
				timePowerUp.spawnPowerUp(player.tempEnemy);
			else
				cout << "Failed to spawn in powerup" << endl;

			gameSounds.playSound(gameSounds.enemyHit, &gameSounds.enemyHitChannel);
			player2.hasHit = false;
		}

		// Plays when a player is hit by an enemy
		if (enemyManager.playerDied == true)
		{
			gameSounds.playSound(gameSounds.playerHit, &gameSounds.playerHitChannel);
			enemyManager.playerDied = false;
		}

		// Plays when a player collects an ammo powerup
		if (ammoPowerUp.collected == true)
		{
			gameSounds.playSound(gameSounds.ammoUp, &gameSounds.ammoUpChannel);
			ammoPowerUp.collected = false;
		}

		// Plays when a player collects a fuel powerup
		if (fuelPowerUp.collected == true)
		{
			gameSounds.playSound(gameSounds.fuelUp, &gameSounds.fuelUpChannel);
			fuelPowerUp.collected = false;
		}

		// Plays when a player collects a time powerup
		if (timePowerUp.collected == true)
		{
			gameSounds.playSound(gameSounds.timeUp, &gameSounds.timeUpChannel);
			timePowerUp.collected = false;
		}

		// Plays sounds for when player activates and deactivates speedboost
		if (player.hasSpedUp == true)
		{
			if (player.hasPlayedSpeedUp == false)
			{
				gameSounds.playSound(gameSounds.speedUp, &gameSounds.speedUpChannel);
				player.hasPlayedSpeedUp = true;
			}
		}

		if (player.hasSpedUp == false)
		{
			if (player.hasPlayedSpeedDown == false)
			{
				gameSounds.playSound(gameSounds.speedDown, &gameSounds.speedDownChannel);
				player.hasPlayedSpeedDown = true;
			}
		}


		if (!player.isTransformed && Player::progress <= 1.0f)
		{
			player.mesh = basicPlayer.mesh;
			player2.mesh = basicPlayer.mesh;

			player.mat = player.baseMat;
			player.projectile.mat = blue;

			player2.mat = player2.baseMat;
			player2.projectile.mat = yellow;
		}

		// For updating the player ships
		for (int i = 0; i < players.size(); i++)
		{

			players[i]->updateProjectiles(&enemyManager.enemyList, players[(i + 1) % 2], &emitters);

			if (players[i]->isAlive())
				players[i]->update(players[(i + 1) % 2]);
			else
			{
				//std::cout << "Player " + std::to_string(i) + "is Dead. " + std::to_string(players[i]->spawnTime) << std::endl;
				players[i]->spawnTime += updateTimer->getElapsedTimeS();

				if ((players[i]->spawnTime >= 2.0f) && (players[i]->numLives > 0))
				{
					std::cout << "ReSpawned" << std::endl;
					players[i]->setLocation(0, 0);
					players[i]->setAlive();
					players[i]->spawnTime = 0.0f;
					players[i]->numLives--;
				}
			}
		}

		if ((player.numLives == 0) && (player2.numLives == 0))
			state = gameOver;

		if ((enemyManager.count == enemyManager.spawnList.size()) && (enemyManager.enemyList.size() == 0))
		{
			//if(!emitters[emitters.size() - 1]->playing)
			//bool temp = false;
			//
			//for (int i = 0; i < emitters.size(); i++)
			//{
			//	if (emitters[i]->playing)
			//		temp = true;
			//}

			//if(!temp)
				//state = win;
		}

	}

	if (state == gameOver)
	{
		if (!empty)
		{
			emptyGame();

			background.gameOver();

			gameSounds.stopSound(&gameSounds.musicChannel);
			gameSounds.playSound(gameSounds.failSound, &gameSounds.failChannel);
		}

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

	if (state == win)
	{
		if (gameSounds.loseHasPlayed == false)
		{
			gameSounds.stopSound(&gameSounds.musicChannel);
			gameSounds.playSound(gameSounds.winSound, &gameSounds.winChannel);
			gameSounds.loseHasPlayed = true;
		}

		emptyGame();
		background.winScreen();
	}

	if (state == score)
	{
		background.leaderBoard();

		if (!setScores)
		{
			SetScores();
			setScores = true;
		}

	}
	draw();

}


void Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < fbos.size(); i++)
	{
		fbos[i]->bindFrameBufferForDrawing();
		fbos[i]->clearFrameBuffer(glm::vec4(0.0f));
		fbos[i]->unbindFrameBuffer(fbos[i]->getWidth(), fbos[i]->getHeight());
	}
	//textShader.bind();

	if (state == main)
	{
		text_fbo.bindFrameBufferForDrawing();
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player.score), -9.5, -8, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Lives: " + std::to_string(player.numLives), -9.5, -7, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player2.score), 7, -8, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Lives: " + std::to_string(player2.numLives), 7, -7, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Ammunition: " + std::to_string(player2.getAmmo()), -9.5, -9, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Ammunition: " + std::to_string(player.getAmmo()), 7, -9, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Fuel: " + std::to_string(player2.getSpeedUp()), -9.5, -10, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Fuel: " + std::to_string(player.getSpeedUp()), 7, -10, .01f, glm::vec3(1, 1, 0));

		if (delay < 5)
		{
			text.RenderText(textShader, cameraOrtho, "LeftStick - Move", -2, 0.5f, 0.01f, glm::vec3(1));
			text.RenderText(textShader, cameraOrtho, "RightStick - Aim/Shoot", -2.5f, -.5f, 0.01f, glm::vec3(1));
		}

		if (Player::progress == Player::transformMax)
			text.RenderText(textShader, cameraOrtho, "Press LB to combine", -2.5f, -7, .01f, glm::vec3(1));

		if (paused == true)
			text.RenderText(textShader, cameraOrtho, "Game Paused", -7.0f, 0.0f, 0.05f, glm::vec3(1.0f, 1.0f, 1.0f));

		text_fbo.unbindFrameBuffer(def.getWidth(), def.getHeight());
	}

	if (state == gameOver)
	{
		text_fbo.bindFrameBufferForDrawing();

		text.RenderText(textShader, cameraOrtho, "Player 1 Score: ", -9.5, -6, .01f, glm::vec3(0, 0, 1));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player.score), -9.5, -7, .01f, glm::vec3(0, 0, 1));

		text.RenderText(textShader, cameraOrtho, "Player 2 Score: ", 5.5f, -6, .01f, glm::vec3(1, 1, 0));
		text.RenderText(textShader, cameraOrtho, "Score: " + std::to_string(player2.score), 5.5f, -7, .01f, glm::vec3(1, 1, 0));

		text_fbo.unbindFrameBuffer(def.getWidth(), def.getHeight());
	}

	if (state == score)
	{
		for (int i = 0; i < scoreValues.size(); i++)
		{
			if (i == player1Spot)
			{
				//string name = name1;
				text.RenderText(textShader, cameraOrtho, std::string(name1) + "    " + std::to_string(scoreValues[i]), -2.5, 3 - (i + 0.2), .02f, glm::vec3(1));
			}

			else if (i == player2Spot)
			{
				//string nameX = name2;
				text.RenderText(textShader, cameraOrtho, std::string(name2) + "    " + std::to_string(scoreValues[i]), -2.5, 3 - (i + 0.2), .02f, glm::vec3(1));
			}
			else
			{
				text.RenderText(textShader, cameraOrtho, scoreNames[i] + "    " + std::to_string(scoreValues[i]), -2.5, 3 - (i + 0.2), .02f, glm::vec3(1));
			}
		}
	}


	//textShader.unbind();
	if (state == title)
	{
		def.bindFrameBufferForDrawing();

		phong.bind();
		playButton.draw(phong, cameraTransform, cameraProjection, pointLights);
		quitButton.draw(phong, cameraTransform, cameraProjection, pointLights);
		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
		phong.unbind();

		def.unbindFrameBuffer(def.getWidth(), def.getHeight());
	}

	else if (state == monologue)
	{

	}

	else if (state == gameOver)
	{
		def.bindFrameBufferForDrawing();
		phong.bind();
		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
		phong.unbind();
		def.unbindFrameBuffer(def.getWidth(), def.getHeight());
	}

	else if (state == win)
	{
		def.bindFrameBufferForDrawing();
		phong.bind();
		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
		phong.unbind();
		def.unbindFrameBuffer(def.getWidth(), def.getHeight());

	}

	else if (state == score)
	{

		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
	}

	else
	{
		particleShader.bind();
		glm::mat4 viewMatrix = glm::lookAt(cameraTranslation, cameraTranslation + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < emitters.size(); i++)
		{
			toBloom.bindFrameBufferForDrawing();
			emitters[i]->draw(viewMatrix, cameraProjection, &particleShader);
			toBloom.unbindFrameBuffer(toBloom.getWidth(), toBloom.getHeight());

			def.bindFrameBufferForDrawing();
			emitters[i]->draw(viewMatrix, cameraProjection, &particleShader);
			def.unbindFrameBuffer(def.getWidth(), def.getHeight());
		}

		particleShader.unbind();

		phong.bind();

		def.bindFrameBufferForDrawing();




		def.unbindFrameBuffer(def.getWidth(), def.getHeight());

		def.bindFrameBufferForDrawing();
		enemyManager.Draw(phong, cameraTransform, cameraProjection, pointLights);

		ammoPowerUp.draw(phong, cameraTransform, cameraProjection, pointLights);

		fuelPowerUp.draw(phong, cameraTransform, cameraProjection, pointLights);

		timePowerUp.draw(phong, cameraTransform, cameraProjection, pointLights);

		def.unbindFrameBuffer(def.getWidth(), def.getHeight());

		if (player.isTransformed && player2.isTransformed)
		{
			def.bindFrameBufferForDrawing();
			player2.shield.draw(phong, cameraTransform, cameraProjection, pointLights);
			def.unbindFrameBuffer(def.getWidth(), def.getHeight());

			for (int i = 0; i < players.size(); i++)
			{
				for (int j = 0; j < players[i]->getProjectiles().size(); j++)
				{
					//if (players[i]->isInvulnerable())
					//{
					//	players[i]->shield.draw(phong, cameraTransform, cameraProjection, pointLights);
					//}

					def.bindFrameBufferForDrawing();
					players[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
					def.unbindFrameBuffer(def.getWidth(), def.getHeight());


					toBloom.bindFrameBufferForDrawing();
					players[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
					toBloom.unbindFrameBuffer(toBloom.getWidth(), toBloom.getHeight());
				}

				def.bindFrameBufferForDrawing();
				player.draw(phong, cameraTransform, cameraProjection, pointLights);
				player.turret.draw(phong, cameraTransform, cameraProjection, pointLights);
				player2.turret.draw(phong, cameraTransform, cameraProjection, pointLights);

				players[i]->blackBar.draw(phong, cameraTransform, cameraProjection, pointLights);
				players[i]->yellowBar.draw(phong, cameraTransform, cameraProjection, pointLights);

				if (players[i]->getTilted())
					players[i]->reticle.draw(phong, cameraTransform, cameraProjection, pointLights);

				def.unbindFrameBuffer(def.getWidth(), def.getHeight());

			}
		}

		else
		{
			for (int i = 0; i < players.size(); i++)
			{
				for (int j = 0; j < players[i]->getProjectiles().size(); j++)
				{
					def.bindFrameBufferForDrawing();
					players[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
					def.unbindFrameBuffer(def.getWidth(), def.getHeight());

					toBloom.bindFrameBufferForDrawing();
					players[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
					toBloom.unbindFrameBuffer(toBloom.getWidth(), toBloom.getHeight());
				}
				if (players[i]->isAlive())
				{
					def.bindFrameBufferForDrawing();
					players[i]->draw(phong, cameraTransform, cameraProjection, pointLights);
					players[i]->turret.draw(phong, cameraTransform, cameraProjection, pointLights);

					players[i]->blackBar.draw(phong, cameraTransform, cameraProjection, pointLights);
					players[i]->yellowBar.draw(phong, cameraTransform, cameraProjection, pointLights);

					if (players[i]->getTilted())
						players[i]->reticle.draw(phong, cameraTransform, cameraProjection, pointLights);

					def.unbindFrameBuffer(def.getWidth(), def.getHeight());
				}
			}
		}

		background_fbo.bindFrameBufferForDrawing();
		background.draw(phong, cameraTransform, cameraProjection, pointLights[0]);
		background_fbo.unbindFrameBuffer(background_fbo.getWidth(), background_fbo.getHeight());
	}


	phong.unbind();

	doBlurPass();

	bloomShader.bind();

	bright.bindFrameBufferForDrawing();
	bright.clearFrameBuffer(glm::vec4(0));

	blur_a.bindTextureForSampling(0, GL_TEXTURE0);
	def.bindTextureForSampling(0, GL_TEXTURE1);

	fullscreenQuad.draw(bloomShader);

	def.unbindTexture(GL_TEXTURE1);
	blur_a.unbindTexture(GL_TEXTURE0);

	bright.unbindFrameBuffer(bright.getWidth(), bright.getHeight());

	bloomShader.unbind();

	orComposite.bind();
	orComposite.sendUniform("u_alpha", true);

	def.bindFrameBufferForDrawing();

	bright.bindTextureForSampling(0, GL_TEXTURE1);
	text_fbo.bindTextureForSampling(0, GL_TEXTURE0);

	fullscreenQuad.draw(orComposite);

	bright.unbindTexture(GL_TEXTURE1);
	text_fbo.unbindTexture(GL_TEXTURE0);
	def.unbindFrameBuffer(def.getWidth(), def.getHeight());

	orComposite.unbind();
	orComposite.bind();
	orComposite.sendUniform("u_alpha", false);

	def.bindTextureForSampling(0, GL_TEXTURE0);
	background_fbo.bindTextureForSampling(0, GL_TEXTURE1);

	FrameBufferObject::clearFrameBuffer(glm::vec4(0));
	FrameBufferObject::unbindFrameBuffer(def.getWidth(), def.getHeight());
	fullscreenQuad.draw(orComposite);

	def.unbindTexture(GL_TEXTURE0);
	background_fbo.unbindTexture(GL_TEXTURE1);
	orComposite.unbind();

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
		//case 27:
		//case 'q':
		//	exit(1);
		//	break;

	case 't':
		std::cout << "Total elapsed time: " << updateTimer->getCurrentTime() << std::endl;
		break;

	case 'r':
	case 'R':
		if (state == gameOver)
		{
			state = main;
			background.restart();
		}
		break;

	case 'l':
	case 'L':
		shouldLightsSpin = !shouldLightsSpin;
		break;
	case 'a':
		debugSelect = true;
		break;
	case 'p':
		paused = !paused;
		pauseTime = 0.0f;
		break;
	case 'b':
	case 'B':
		players[0]->setDead();//enemyManager.bossSpawn = true;
	default:
		break;
	}

	//Getting the players inputs for the leaderboards
	if (state == score)
	{
		if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122) || (key >= 48 && key <= 57))
		{
			if (!playerTwoType)
			{
				if (itr1 < 3)
				{
					name1[itr1] = key;
					itr1++;
				}
				else
				{
					playerTwoType = true;
					itr1 = 0;
				}
			}
			else
			{
				if (itr2 < 3)
				{
					name2[itr2] = key;
					itr2++;
				}
				else
				{
					SaveScores();
					exit(1);
				}
			}
		}


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
			//delete players[i]->projectiles[j];
		}

		//players[i]->projectiles.clear();
	}

	background.gameOver();

	player.playerState = player.state::alive;
	player2.playerState = player.state::alive;

	selected = none;

	background.gameOver();

	empty = true;
	gameSounds.hasPlayed = false;
}

void Game::doBrightPass()
{
	bright.bindFrameBufferForDrawing();
	bright.clearFrameBuffer(glm::vec4(0.4f, 0.4f, 0.5f, 1.0f));

	brightPass.bind();
	brightPass.sendUniform("u_bloomThreshold", 0.5f);

	toBloom.bindTextureForSampling(0, GL_TEXTURE0);
	fullscreenQuad.draw(brightPass);
	toBloom.unbindTexture(GL_TEXTURE0);

	brightPass.unbind();
	bright.unbindFrameBuffer(bright.getWidth(), bright.getHeight());
}

void Game::doBlurPass()
{
	lowRes.bindFrameBufferForDrawing();
	lowRes.clearFrameBuffer(glm::vec4(0.0f));

	brightPass.bind();
	brightPass.sendUniform("u_bloomThreshold", 0.2f);

	toBloom.bindTextureForSampling(0, GL_TEXTURE0);
	fullscreenQuad.draw(brightPass);
	toBloom.unbindTexture(GL_TEXTURE0);

	brightPass.unbind();
	lowRes.unbindFrameBuffer(lowRes.getWidth(), lowRes.getHeight());

	blur_a.bindFrameBufferForDrawing();
	blur_a.clearFrameBuffer(glm::vec4(0.0f));

	blurShader.bind();
	blurShader.sendUniform("u_texelSize", glm::vec4(1 / lowRes.getWidth(), 1 / lowRes.getHeight(), 0.0f, 0.0f));

	lowRes.bindTextureForSampling(0, GL_TEXTURE0);
	fullscreenQuad.draw(blurShader);
	lowRes.unbindTexture(GL_TEXTURE0);

	blur_a.unbindFrameBuffer(blur_a.getWidth(), blur_a.getHeight());
	blurShader.unbind();

	int numBlurs = 5;

	//for (int i = 0; i < numBlurs; i++)
	//{
	//	//Blur Buffer A
	//	blurShader.bind();
	//	blurShader.sendUniform("u_texelSize", glm::vec4(1 / blur_b.getWidth(), 1 / blur_b.getHeight(), 0.0f, 0.0f));
	//	blur_b.bindFrameBufferForDrawing();
	//	blur_b.clearFrameBuffer(glm::vec4());
	//
	//	blur_a.bindTextureForSampling(0, GL_TEXTURE0);
	//	fullscreenQuad.draw(blurShader);
	//	blur_a.unbindTexture(GL_TEXTURE0);
	//
	//	blur_b.unbindFrameBuffer(blur_b.getWidth(), blur_b.getHeight());
	//	blurShader.unbind();
	//
	//	//Blur Buffer B
	//	blurShader.bind();
	//	blurShader.sendUniform("u_texelSize", glm::vec4(1 / blur_a.getWidth(), 1 / blur_a.getHeight(), 0.0f, 0.0f));
	//	blur_a.bindFrameBufferForDrawing();
	//	blur_a.clearFrameBuffer(glm::vec4());
	//
	//	blur_b.bindTextureForSampling(0, GL_TEXTURE0);
	//	fullscreenQuad.draw(blurShader);
	//	blur_b.unbindTexture(GL_TEXTURE0);
	//
	//	blur_a.unbindFrameBuffer(blur_a.getWidth(), blur_a.getHeight());
	//	blurShader.unbind();
	//}
}

//Loading the text files and storing the scores and names in the vectors
void Game::LoadScores()
{
	file.open("score.txt", std::fstream::in);
	if (file.fail())
	{
		std::cout << "Failed to load file" << std::endl;
	}
	string line;
	string name = "", scoreText;
	if (file)
	{
		while (!file.eof())
		{
			getline(file, line);
			if (line != "")
			{
				name = line.substr(0, 3);

				scoreText = line.substr(3, 7);

				int x = stoi(scoreText);

				scoreNames.push_back(name);
				scoreValues.push_back(x);
			}
		}
	}

	int j;

	for (int i = 0; i < scoreValues.size(); i++)
	{
		j = i;
		while (j > 0 && scoreValues[j] > scoreValues[j - 1])
		{
			int valueTemp = scoreValues[j];
			string nameTemp = scoreNames[j];

			scoreValues[j] = scoreValues[j - 1];
			scoreNames[j] = scoreNames[j - 1];

			scoreValues[j - 1] = valueTemp;
			scoreNames[j - 1] = nameTemp;
			j--;
		}
	}

	file.close();
}


//Setting the scores once on runtime
void Game::SetScores()
{
	//If the players score is high enough, put them on the board
	if (players[0]->score > scoreValues[scoreValues.size() - 1])
	{
		scoreValues.push_back(players[0]->score);
		scoreNames.push_back("1");				   //Players 1's default name for now


		int j;
		for (int i = 0; i < scoreValues.size(); i++) // Sorting the the list with the new player score entry (insertion sort)
		{
			j = i;
			while (j > 0 && scoreValues[j] > scoreValues[j - 1])
			{
				int valueTemp = scoreValues[j];
				string nameTemp = scoreNames[j];

				scoreValues[j] = scoreValues[j - 1];
				scoreNames[j] = scoreNames[j - 1];

				scoreValues[j - 1] = valueTemp;
				scoreNames[j - 1] = nameTemp;
				j--;
			}
		}

		for (int i = 0; i < scoreNames.size(); i++)
		{
			//In the draw, we need to know which spot the player is on the list
			if (scoreNames[i] == "1")
			{
				player1Spot = i;
				break;
			}
		}

		//Pop the last value in the sorted list
		scoreValues.pop_back();
		scoreNames.pop_back();
	}

	//Exact same as above but for player 2
	if (players[1]->score > scoreValues[scoreValues.size() - 1])
	{
		scoreValues.push_back(players[1]->score);
		scoreNames.push_back("2");

		int j;
		for (int i = 0; i < scoreValues.size(); i++)
		{
			j = i;
			while (j > 0 && scoreValues[j] > scoreValues[j - 1])
			{
				int valueTemp = scoreValues[j];
				string nameTemp = scoreNames[j];

				scoreValues[j] = scoreValues[j - 1];
				scoreNames[j] = scoreNames[j - 1];

				scoreValues[j - 1] = valueTemp;
				scoreNames[j - 1] = nameTemp;
				j--;
			}
		}


		for (int i = 0; i < scoreNames.size(); i++)
		{
			if (scoreNames[i] == "2")
			{
				player2Spot = i;
				break;
			}
		}
		scoreValues.pop_back();
		scoreNames.pop_back();
	}
}

void Game::SaveScores()
{
	file.open("score.txt", std::fstream::out);
	if (file.fail())
	{
		std::cout << "Failed to load file" << std::endl;
	}

	if (file)
	{
		for (int i = 0; i < scoreValues.size(); i++)
		{
			if (i == player1Spot)
				file << std::string(name1) << " " << scoreValues[i] << std::endl;
			else if (i == player2Spot)
				file << std::string(name2) << " " << scoreValues[i] << std::endl;
			else
				file << scoreNames[i] << " " << scoreValues[i] << std::endl;
		}

		file.close();
	}
}
