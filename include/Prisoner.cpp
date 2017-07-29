#include "Prisoner.h"
#include "Helpers.h"
#include <iostream>

Prisoner::Prisoner(const sf::Vector2f & startPosition) 
	: GameObject(Type::Prisoner)
	, m_collided(false) {
	m_position = startPosition;
}

void Prisoner::update(float dt, const sf::Vector2f & playerPosition, const sf::Vector2f& playerSize) {
	
	m_direction = Helpers::normaliseCopy(playerPosition - m_position);
	sf::Vector2f velocity = m_direction * MOVE_SPEED;
	Helpers::limit(velocity, MOVE_SPEED);
	m_position += velocity * dt;
	GameObject::update(dt);

	float distanceBetween = Helpers::getLength(playerPosition - m_position);
	//its a square so just use the width as if its a radius
	if (distanceBetween < (m_sprite.getGlobalBounds().width * 0.5f) + (playerSize.x)) {
		m_collided = true;
		std::cout << "pirsoner hit you" << std::endl;
	}
}

bool Prisoner::getCollided() const {
	return m_collided;
}
