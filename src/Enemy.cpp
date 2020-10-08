#include "Enemy.h"
#include "TextureManager.h"

Enemy::Enemy() {
	TextureManager::Instance()->load("../Assets/textures/StormTrooper3.png", "StormTrooper");

	auto size = TextureManager::Instance()->getTextureSize("StormTrooper");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(535.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);
}

Enemy::~Enemy()
= default;

void Enemy::draw() {
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("StormTrooper", x, y, 0, 255, true);
}

void Enemy::update() {

}

void Enemy::clean() {

}