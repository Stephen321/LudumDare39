#pragma once

#include "GameObject.h"

class Prisoner : public GameObject {
public:
	Prisoner(const sf::Vector2f& startPosition, const sf::Vector2f& wayPointPosition);
	void update(float dt, const sf::Vector2f & playerPosition);
	bool getCollided(const sf::Vector2f & playerPosition, const sf::Vector2f& playerSize);
	int decreaseHealth();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	const float MOVE_SPEED = 170.f;
	sf::Vector2f m_wayPointPosition;
	bool m_waypointReached;
	bool m_collided;
	int m_health;


	bool m_showBlood;
	sf::Sprite m_bloodTex;
};
