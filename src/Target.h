#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	//Math related functions
	float Target::checkDistance(GameObject* pGameObject);
	float calculateTheta();
	float changeX();
	float changeY();
	float calculateForce();
	void recalculateProjectile();
	
	//Getters
	glm::vec2 getDirection();
	glm::vec2 getEnemyPosition();
	bool getUpdateDetenator();
	float getDistanceToEnemy();
	float getSpeedThrown();
	float getGravity();
	float getForce();
	float getTheta();
	float getMass();
	
	//Setters
	void setDirection(glm::vec2);
	void setEnemyPosition(glm::vec2);
	void setUpdateDetenator(bool);
	void setDistanceToEnemy(float);
	void setSpeedThrown(float);
	void setGravity(float);
	void setForce(float);
	void setTheta(float);
	void setMass(float);



private:
	glm::vec2 m_direction;
	glm::vec2 m_enemyPosition;
	bool m_updateDetenator;
	float m_distanceToEnemy;
	float m_speedThrown;
	float m_gravity;
	float m_force;
	float m_theta;
	float m_mass;

	void m_move();
	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */