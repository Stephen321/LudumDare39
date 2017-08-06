#include "BulletPool.h"

#include "GameData.h"
#include "BulletPool.h"


BulletPool::BulletPool(int size, AudioManager& audioManager)
	: m_size(size)
	, m_audioManager(audioManager) {
	m_bullets = std::vector<Bullet>(m_size, Bullet());
}

bool BulletPool::fire(const sf::Vector2f & start, const sf::Vector2f & direction) {
	GameData& data = GameData::getInstance();

	for (int i = 0; i < m_size; i++) {
		if (m_bullets[i].getActive() == false) {
			m_bullets[i].fire(start, direction);
			m_audioManager.play(AudioManager::Type::Gunshot);
			return true;
		}
	}
	return false;
}

void BulletPool::update(float dt) {
	for (int i = 0; i < m_size; i++) {
		if (m_bullets[i].getActive())
			m_bullets[i].update(dt);
	}
}

void BulletPool::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	for (int i = 0; i < m_size; i++) {
		if (m_bullets[i].getActive())
			target.draw(m_bullets[i], states);
	}
}

std::vector<Bullet>& BulletPool::getBullets() {
	return m_bullets;
}

void BulletPool::reset() {
	m_bullets.clear();
	m_bullets = std::vector<Bullet>(m_size, Bullet());
}
