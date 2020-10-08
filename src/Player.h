#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "Sprite.h"
#include "Label.h"
#include "CollisionManager.h"
#include "Enemy.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMoving();
	void stopMovingX();
	void stopMovingY();
	bool checkCollision(GameObject* pGameObject);
	
	float checkDistance(GameObject* pGameObject);
	glm::vec2 m_direction;
private:
	const float ACCELERATION = 10.0f;
};

#endif /* defined (__PLAYER__) */