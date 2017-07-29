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
	sf::Vector2f getSize() const;
	void setPositionX(float x);
	void setPositionY(float y); 
private:
	const float MOVE_SPEED = 175.f;
	const float RELOAD_TIME = 0.33f;
	bool m_firing;
	BulletPool& m_bulletPool;
	float m_reloadTimer;
};
