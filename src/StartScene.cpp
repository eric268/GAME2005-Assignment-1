#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("Background2", 400, 300, 0, 255, true);
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/Background2.jpg", "Background2");

	const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color white = { 225, 255, 255, 255 };
	m_pStartLabel = new Label("Motion in 1 and 2 Directions", "Consolas", 45, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);
	
	m_pStartLabel2 = new Label("Physics Simulation", "Consolas", 45, blue, glm::vec2(400.0f, 80.0f));
	m_pStartLabel2->setParent(this);
	addChild(m_pStartLabel2);

	m_pInstructionsLabel = new Label("Press start to Play", "Consolas", 30, blue, glm::vec2(400.0f, 200.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	
	m_pName[0]  = new Label("Eric Galway 101252535", "Consolas", 15, white, glm::vec2(700.0f, 570.0f));
	m_pName[0]->setParent(this);
	addChild(m_pName[0]);

	m_pName[1] = new Label("Ian Chapman 101248866", "Consolas", 15, white, glm::vec2(700.0f, 585.0f));
	m_pName[1]->setParent(this);
	addChild(m_pName[1]);
	

	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position = glm::vec2(400.0f, 510.0f);
	addChild(m_pPlayer);

	for (int i = 0; i < 4; i++)
	{
		m_arrEnemies[i] = new Enemy();
		m_arrEnemies[i]->getTransform()->position = glm::vec2(150.0f + i*160.0f, 510.0f);
		addChild(m_arrEnemies[i]);
	}

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	
}

