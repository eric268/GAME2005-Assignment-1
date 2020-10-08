#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Enemy.h"
#include "Target.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	float GuiSliderPlaceholders[6];

	void setGuidSlidePlaceholders();
	void resetSceneSettings();
	void checkGuiChangs();

	void setEnemyPositionInPlayScene();

private:
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Enemy* m_pEnemy[3];
	
	Label* m_pDistanceLabel;
	Label* m_pMassLabel;
	Label* m_pVelocityLabel;
	Label* m_pAccelerationLabel;
	Label* m_pForceLabel;
	Label* m_pPPM;
	Label* m_pCannotHitTrooper;
	
	Button* m_pBackButton;
	Button* m_pNextButton;
	Target* m_pThermalDetonator;



	// ImGui utility functions
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
	void m_updateUI();

	// ImGui menu variables
	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	// ImGui button variables
	//ImVec4 m_manhattanButtonColour;
	//ImVec4 m_euclideanButtonColour;

	// ImGui visibility variables
	bool hitStormtrooper = true;
	bool useAngleChosen = false;
};

#endif /* defined (__PLAY_SCENE__) */