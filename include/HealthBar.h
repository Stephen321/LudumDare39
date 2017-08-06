#pragma once

#include "SFML/Graphics.hpp" 

class HealthBar : public sf::Drawable {
public:
	HealthBar();
	HealthBar(float maxHealth, const sf::Color& backgroundOutline, const sf::Color& background, const sf::Color& foreground);
	bool changeHealth(float change);
	float getHealth() const;
	float getMaxHealth() const;
	void setPosition(const sf::Vector2f& position);
	void reset();
	void setYOffset(float yOffset);
	void setXSize(float xSize);
	void setYSize(float ySize);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
private:
	sf::RectangleShape m_backgroundRect;
	sf::RectangleShape m_healthRect;
	float m_yOffset;
	float m_maxHealth;
	float m_currentHealth;
	const int LERP_MIN = 5;
	const float LERP_TIME = 0.2f;
	float m_timer;
	float m_lerpStart;
	float m_lerpEnd;
};