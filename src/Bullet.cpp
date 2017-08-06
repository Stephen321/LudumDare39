#include "Bullet.h"
#include "Helpers.h"

Bullet::Bullet() :
	GameObject(Type::Bullet){
	m_active = false;
	m_position.x = -1000.f;
	m_position.y = -1000.f;
	GameObject::update(0.f);
}

void Bullet::fire(sf::Vector2f startPosition, sf::Vector2f direction) {
	m_position = startPosition;
	m_velocity = direction * MOVE_SPEED;
	Helpers::limit(m_velocity, MOVE_SPEED);
	m_distance = 0.f;
	m_active = true;
}

void Bullet::update(float dt) {
	if (!m_active)
		return;
	m_position += m_velocity * dt;
	m_distance += MOVE_SPEED * dt;
	if (m_distance > MAX_DISTANCE) {
		m_active = false;
		m_position.x = -1000.f;
		m_position.y = -1000.f;
	}
	GameObject::update(dt);
}

sf::Vector2f Bullet::getVelocity() const {
	return m_velocity;
}
