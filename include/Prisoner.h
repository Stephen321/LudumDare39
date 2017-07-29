#pragma once

#include "GameObject.h"

class Prisoner : public GameObject {
public:
	Prisoner(const sf::Vector2f& startPosition);
	void update(float dt, const sf::Vector2f& playerPosition, const sf::Vector2f& playerSize); //returns if it collided with the player or not
	bool getCollided() const;
private:
	const float MOVE_SPEED = 125.f;
	bool m_collided;
};
