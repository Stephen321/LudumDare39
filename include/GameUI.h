#pragma once

#include "SFML\Graphics.hpp"
#include "HealthBar.h"

class GameUI : public sf::Drawable {
public:
	GameUI(const sf::Vector2u windowSize);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void changeHealth(float change);
private:
	HealthBar m_health;
	sf::Vector2u m_windowSize;
	const float HealthXPos = 0.563;
	const float HealthXSize = 0.15;
	const float HealthYSize = 0.022;
};