#include "BloodManager.h"

BloodManager::BloodManager() {
}

void BloodManager::spawnBlood(const sf::Vector2f & position, float rotation) {
	GameObject blood(GameObject::Type::Blood);
	blood.setPositionX(position.x);
	blood.setPositionY(position.y);
	blood.setRotation(rotation);
	m_bloods.push_back(blood);
}

void BloodManager::update(float dt) {
	for (int i = 0; i < m_bloods.size(); i++) {
		m_bloods[i].update(dt);
	}
}

void BloodManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	for (int i = 0; i < m_bloods.size(); i++) {
		target.draw(m_bloods[i], states);
	}
}

void BloodManager::reset() {
	m_bloods.clear();
}
