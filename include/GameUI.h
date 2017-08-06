#pragma once

#include "SFML\Graphics.hpp"
#include "HealthBar.h"

class GameUI : public sf::Drawable {
public:
	GameUI(const sf::Vector2u windowSize);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool changeHealth(float change);
	bool changePower(float change);
	float getPower() const;
	float getMaxPower() const;
	void reset(int level = 1);
	void setPrisonerInfo(int remaining, int max);
	void decreasePowerLevel();
	void update(float dt);

private:
	HealthBar m_health;
	HealthBar m_power;
	sf::Vector2u m_windowSize;
	const float HealthXPos = 0.563;
	const float HealthXSize = 0.15;
	const float HealthYSize = 0.022;
	const float PowerXPos = 0.288;
	sf::Text m_levelTxt;
	sf::Text m_prisonersTxt;
	sf::Sprite m_panel;
	sf::Sprite m_powerIcon;
	sf::Sprite m_healthIcon;
	sf::Sprite m_powerOverlay;
	int m_powerLevel;

	sf::Sprite m_highPwrOverlay;
	sf::Sprite m_mediumPwrOverlay;
	sf::Sprite m_lowPwrOverlay;
	sf::Sprite m_noPwrOverlay;
};