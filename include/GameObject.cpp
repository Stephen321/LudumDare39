#include "GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameData.h"
GameObject::GameObject(Type type) :
	m_active(true) {
	GameData& data = GameData::getInstance();
	//TODO: use spritesheet instead
	switch (type) {
	case Type::Player:
		m_sprite.setTexture(data.playerTexture);
		break;
	case Type::Prisoner:
		m_sprite.setTexture(data.prisonerTexture);
		break;
	case Type::Bullet:
		m_sprite.setTexture(data.bulletTexture);
		break;
	default:
		break;
	}
	if (data.textureScaler != 1.f) {
		m_sprite.setScale(data.textureScaler, data.textureScaler);
	}
	m_sprite.setOrigin(m_position.x + m_sprite.getGlobalBounds().width * 0.5f
					 , m_position.y + m_sprite.getGlobalBounds().height * 0.5f);
}

void GameObject::update(float dt) {
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(atan2f(m_direction.y, m_direction.x) * (180.f / M_PI));
}

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (m_active)
		target.draw(m_sprite);
	else //debug
	{
		sf::Sprite s = m_sprite;
		s.setColor(sf::Color(255, 0, 0));
		target.draw(s);
	}
}

sf::Vector2f GameObject::getPosition() const {
	return m_position;
}

void GameObject::setActive(bool active) {
	m_active = active;
}

bool GameObject::getActive() const {
	return m_active;
}
