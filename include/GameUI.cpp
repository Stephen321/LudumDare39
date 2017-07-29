#include "GameUI.h"
#include "Constants.h"

GameUI::GameUI(const sf::Vector2u windowSize) :
	m_windowSize(windowSize)
	, m_health(PLAYER_HEALTH) {
	m_health.setXSize(HealthXSize * windowSize.x);
	m_health.setYSize(HealthYSize * windowSize.y);
	m_health.update(sf::Vector2f(m_windowSize.x * HealthXPos, 5));
}

void GameUI::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_health);
}

void GameUI::changeHealth(float change) {
	m_health.changeHealth(change);
}
