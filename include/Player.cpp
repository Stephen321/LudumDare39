#include "Player.h"
#include "Helpers.h"
Player::Player(const sf::Vector2f& startPosition, BulletPool& bulletPool) :
	GameObject(Type::Player)
	, m_firing(false)
	, m_bulletPool(bulletPool)
	, m_reloadTimer(0.f) {
	m_position = startPosition;
}

void Player::update(float dt) {
	sf::Vector2f velocity;
	//check keyboard state 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		velocity.y += -MOVE_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		velocity.x += -MOVE_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		velocity.y += MOVE_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		velocity.x += MOVE_SPEED;
	}
	Helpers::limit(velocity, MOVE_SPEED);
	m_position += velocity * dt;

	m_reloadTimer += dt;
	if (m_firing) {
		if (m_reloadTimer > RELOAD_TIME) {
			m_reloadTimer = 0.f;
			sf::Vector2f bulletPos = m_position; //our centre
			bulletPos += Helpers::normaliseCopy(m_direction) *
						(m_sprite.getGlobalBounds().width * 0.5f); //
			m_bulletPool.fire(bulletPos, m_direction);
		}

	}
	GameObject::update(dt);
}

void Player::setDirection(sf::Vector2f direction) {
	m_direction = direction;
}

void Player::setFiring(bool firing) {
	m_firing = firing;
}

bool Player::getFiring() const {
	return m_firing;
}

