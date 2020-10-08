#include "Target.h"
#include "TextureManager.h"
#include "Util.h"
#include <iostream>
#define PI 3.14159265

Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/ThermalDetonator.png","ThermalDetonator");

	const auto size = TextureManager::Instance()->getTextureSize("ThermalDetonator");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(50.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	m_direction = glm::vec2(0.0f, 0.0f);
	m_theta = 0.0f;
	m_speedThrown = 95.0f;
	m_gravity = 9.8f;
	m_mass = 2.2f;
	m_updateDetenator = false;
	m_force = 0.0f;
	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("ThermalDetonator", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
}
float Target::checkDistance(GameObject* pGameObject) {
	int x = getTransform()->position.x - pGameObject->getTransform()->position.x;
	int y = getTransform()->position.y - pGameObject->getTransform()->position.y;
	return sqrt(pow(x, 2) + pow(y, 2));
}

float Target::calculateTheta()
{
	m_theta = 0.5 * (asin((m_distanceToEnemy * m_gravity) / (m_speedThrown * m_speedThrown))*180/3.14159265);
	return m_theta;
}

float Target::changeX()
{
	float temp =m_theta*PI/180;
	temp = cos(temp);
	m_direction.x = m_speedThrown * temp;
	if (getTransform()->position.x > m_enemyPosition.x)
	{
		m_direction *= -1;
	}
	return m_direction.x;

}

float Target::changeY()
{
	float temp = m_theta * PI / 180;
	temp = sin(temp);
	m_direction.y = -(m_speedThrown * temp);
	return m_direction.y;
}

float Target::calculateForce()
{
	m_force = m_mass * getRigidBody()->acceleration.y;
	return m_force;
}

void Target::recalculateProjectile()
{
	calculateTheta();
	changeX();
	changeY();
}

glm::vec2 Target::getDirection()
{
	return m_direction;
}

void Target::setDirection(glm::vec2 direction)
{
	m_direction = direction;
}

glm::vec2 Target::getEnemyPosition()
{
	return m_enemyPosition;
}

void Target::setEnemyPosition(glm::vec2 enemyPosition)
{
	m_enemyPosition = enemyPosition;
}

bool Target::getUpdateDetenator()
{
	return m_updateDetenator;
}

void Target::setUpdateDetenator(bool updateDetenator)
{
	m_updateDetenator = updateDetenator;
}

float Target::getDistanceToEnemy()
{
	return m_distanceToEnemy;
}

void Target::setDistanceToEnemy(float distanceToEnemy)
{
	m_distanceToEnemy = distanceToEnemy;
}

float Target::getSpeedThrown()
{
	return m_speedThrown;
}

void Target::setSpeedThrown(float speedThrown)
{
	m_speedThrown = speedThrown;
}

float Target::getGravity()
{
	return m_gravity;
}

void Target::setGravity(float gravity)
{
	m_gravity = gravity;
}

float Target::getForce()
{
	return m_force;
}

void Target::setForce(float force)
{
	m_force = force;
}

float Target::getTheta()
{
	return m_theta;
}

void Target::setTheta(float theta)
{
	m_theta = theta;
}

float Target::getMass()
{
	return m_mass;
}

void Target::setMass(float mass)
{
	m_mass = mass;
}

void Target::clean()
{
}

void Target::m_move()
{
	if (m_distanceToEnemy > 3 && m_updateDetenator)
	{
		const float deltaTime = 1.0f / 60.f;

		getRigidBody()->acceleration.y = m_gravity;
		m_direction.y += getRigidBody()->acceleration.y * deltaTime;
		
		getRigidBody()->velocity.y = m_direction.y;
		getRigidBody()->velocity.x = m_direction.x;
		
		glm::vec2 pos = getTransform()->position;
		pos.x += getRigidBody()->velocity.x * deltaTime;
		pos.y += getRigidBody()->velocity.y * deltaTime;
		getTransform()->position = pos;
	}
	else if (m_distanceToEnemy <= 3 && m_updateDetenator)
	{
		m_updateDetenator = false;
		getRigidBody()->acceleration = glm::vec2(0.f);
		getRigidBody()->velocity = glm::vec2(0.0f);
		m_distanceToEnemy = 0;
		getTransform()->position = m_enemyPosition;

	}
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
