#include "Prisoner.h"
#include "Helpers.h"
#include "Constants.h"
#include <iostream>

Prisoner::Prisoner(const sf::Vector2f & startPosition, const sf::Vector2f& wayPointPosition) 
	: GameObject(Type::Prisoner)
	, m_health(PRISONER_HEALTH)
	, m_waypointReached(false)
	, m_wayPointPosition(wayPointPosition) {
	m_position = startPosition;
}

void Prisoner::update(float dt, const sf::Vector2f & playerPosition) {
	sf::Vector2f velocity;
	if (m_waypointReached) {
		m_direction = Helpers::normaliseCopy(playerPosition - m_position);
		velocity = m_direction * MOVE_SPEED;
		Helpers::limit(velocity, MOVE_SPEED);
	}
	else {
		if (Helpers::getLength(m_wayPointPosition - m_position) < 10.f) {
			m_waypointReached = true;
		}
		m_direction = Helpers::normaliseCopy(m_wayPointPosition - m_position);
		velocity = m_direction * MOVE_SPEED;
		Helpers::limit(velocity, MOVE_SPEED);
	}
	m_position += velocity * dt;
	GameObject::update(dt);
}

bool Prisoner::getCollided(const sf::Vector2f & playerPosition, const sf::Vector2f& playerSize) {
	bool collided = false;
	float distanceBetween = Helpers::getLength(playerPosition - m_position);
	//its a square so just use the width as if its a radius
	if (distanceBetween < (playerSize.x * 0.5f) + (m_sprite.getGlobalBounds().width * 0.5f)) {
		collided = true;
		m_active = false;
	}
	return collided;
}

void Prisoner::decreaseHealth() {
	m_health--;
	if (m_health <= 0) {
		//dead
		m_active = false;
	}
}
