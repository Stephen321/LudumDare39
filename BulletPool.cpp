#include "BulletPool.h"

#include "GameData.h"
#include "BulletPool.h"


BulletPool::BulletPool(int size)
	: m_size(size) {
	m_bullets = std::vector<Bullet>(m_size, Bullet());
}

bool BulletPool::fire(const sf::Vector2f & start, const sf::Vector2f & direction) {
	GameData& data = GameData::getInstance();

	for (int i = 0; i < m_size; i++) {
		if (m_bullets[i].getActive() == false) {//found unused bullet
			m_bullets[i].fire(start, direction);
			return true;
		}
	}
	return false;
	//Couldnt find a free bullet, consider making the size bigger
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
			target.draw(m_bullets[i]);
	}
}

std::vector<Bullet>& BulletPool::getBullets() {
	return m_bullets;
}

void BulletPool::reset() {
	m_bullets.clear();
	m_bullets = std::vector<Bullet>(m_size, Bullet());
}
