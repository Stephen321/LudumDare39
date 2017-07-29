#pragma once

#include "GameObject.h"

class Bullet : public GameObject {
public:
	Bullet();
	void fire(sf::Vector2f startPosition, sf::Vector2f direction);
	void update(float dt) override;
private:
	const float MOVE_SPEED = 350.f;
	const int MAX_DISTANCE = 850;
	sf::Vector2f m_velocity;
	bool m_firing;
	float m_distance;
};
