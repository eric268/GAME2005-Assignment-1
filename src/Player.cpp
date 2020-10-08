#include "Player.h"
#include "TextureManager.h"
#include "Util.h"

Player::Player()
{
	TextureManager::Instance()->load("../Assets/textures/wookie.png", "Wookie");
	
	auto size = TextureManager::Instance()->getTextureSize("Wookie");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(50.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("Wookie", x, y, 0, 255, true);
}

void Player::update()
{
	//const float deltaTime = 1.0f / 60.f;

	////Normalize direction vector
	//float dirMagnitude = Util::magnitude(m_direction);
	//if (dirMagnitude)
	//{
	//	m_direction *= 1.0f / dirMagnitude;

	//	getRigidBody()->acceleration = Util::normalize(m_direction) * ACCELERATION;

	//}
	//else if (Util::magnitude(getRigidBody()->velocity) >0)
	//{
	//	getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -ACCELERATION;
	//}
	//if (!dirMagnitude)
	//{
	//	if (Util::magnitude(getRigidBody()->velocity) < 10.0f)
	//	{
	//		getRigidBody()->acceleration = glm::vec2(0.f);
	//		getRigidBody()->velocity = glm::vec2(0.0f);
	//	}
	//}


	//	getRigidBody()->velocity += getRigidBody()->acceleration;
	//	glm::vec2 pos = getTransform()->position;


	//	pos.x += getRigidBody()->velocity.x * deltaTime;
	//	pos.y += getRigidBody()->velocity.y * deltaTime;

	//	getTransform()->position = pos;
	


	
}

void Player::clean()
{

}

void Player::moveLeft() {
	m_direction.x = -1;
}

void Player::moveRight() {
	m_direction.x = 1;
}

void Player::moveUp()
{
	m_direction.y = -1;
}

void Player::moveDown()
{
	m_direction.y = 1;
}

void Player::stopMoving()
{
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
}

void Player::stopMovingX()
{
	m_direction.x = 0;
}

void Player::stopMovingY()
{
	m_direction.y = 0;
}

bool Player::checkCollision(GameObject* pGameObject)
{	
	if (checkDistance(pGameObject) <= getWidth() / 2 + pGameObject->getWidth() / 2)
	{
		getRigidBody()->isColliding = true;
		return true;
	}
	else
	{
		getRigidBody()->isColliding = false;
		return false;
	}
}

float Player::checkDistance(GameObject* pGameObject) {
	int x = getTransform()->position.x - pGameObject->getTransform()->position.x;
	int y = getTransform()->position.y - pGameObject->getTransform()->position.y;
	return sqrt(pow(x, 2) + pow(y, 2));
}
