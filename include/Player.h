#pragma once

#include "GameObject.h"
#include "BulletPool.h"

class Player : public GameObject {
public:
	Player(const sf::Vector2f& startPosition, BulletPool& bulletPool);
	void update(float dt) override;
	void setDirection(sf::Vector2f direction);
	void setFiring(bool firing);
	bool getFiring() const; 
	void reset();

private:
	const float MOVE_SPEED = 200.f;
	const float RELOAD_TIME = 0.15f;
	float m_startX;
	bool m_firing;
	BulletPool& m_bulletPool;
	float m_reloadTimer;
};
