#include "Prisoner.h"
#include "Helpers.h"
#include "Constants.h"
#include "GameData.h"

Prisoner::Prisoner(const sf::Vector2f & startPosition, const sf::Vector2f& wayPointPosition)
	: GameObject(Type::Prisoner)
	, m_health(PRISONER_HEALTH)
	, m_waypointReached(false)
	, m_wayPointPosition(wayPointPosition)
	, m_showBlood(false) {
	m_position = startPosition;

	m_bloodTex.setTexture(GameData::getInstance().prisonerBloodTex);
	if (GameData::getInstance().textureScaler != 1.f) {
		m_bloodTex.setScale(GameData::getInstance().textureScaler, GameData::getInstance().textureScaler);
	}
	m_bloodTex.setOrigin(m_bloodTex.getGlobalBounds().width * 0.5f
					   , m_bloodTex.getGlobalBounds().height * 0.5f);
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

	if (m_showBlood) {
		m_bloodTex.setPosition(m_position);
		m_bloodTex.setRotation(m_sprite.getRotation());
	}
	GameObject::update(dt);
}

bool Prisoner::getCollided(const sf::Vector2f & playerPosition, const sf::Vector2f& playerSize) {
	bool collided = false;
	float distanceBetween = Helpers::getLength(playerPosition - m_position);
	if (distanceBetween < (playerSize.x * 0.5f) + (m_sprite.getGlobalBounds().width * 0.5f)) {
		collided = true;
		m_active = false;
	}
	return collided;
}

int Prisoner::decreaseHealth() {
	m_health--;
	m_showBlood = true;
	if (m_health <= 0) {
		m_active = false;
	}
	return m_health;
}

void Prisoner::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	GameObject::draw(target, states);
	if (m_showBlood)
		target.draw(m_bloodTex, states);
}
