#include "GameUI.h"
#include "Constants.h"
#include "GameData.h"

GameUI::GameUI(const sf::Vector2u windowSize) :
	  m_windowSize(windowSize)
	, m_health(PLAYER_HEALTH, sf::Color(25, 25, 25, 255), sf::Color(255, 100, 100), sf::Color(25, 240, 25))
	, m_power((MAX_POWER + 1) * POWER_POINTS_PER_LEVEL, sf::Color(25, 25, 25, 255), sf::Color(201, 188, 245), sf::Color(255, 255, 50)) {
	m_health.setXSize(HealthXSize * windowSize.x);
	m_health.setYSize(HealthYSize * windowSize.y);
	m_health.setPosition(sf::Vector2f(m_windowSize.x * HealthXPos, 5.f));


	m_power.setXSize(HealthXSize * windowSize.x);
	m_power.setYSize(HealthYSize * windowSize.y);
	m_power.setPosition(sf::Vector2f(m_windowSize.x * PowerXPos, 5.f));

	m_levelTxt = sf::Text("", GameData::getInstance().levelFont, 18.f);
	m_levelTxt.setFillColor(sf::Color(50,50,50));
	m_levelTxt.setPosition(TILE_SIZE * m_windowSize.x, 5.f);
	m_levelTxt.setStyle(sf::Text::Regular);
	m_levelTxt.setString("Level: 1");

	m_prisonersTxt = sf::Text("", GameData::getInstance().levelFont, 18.f);
	m_prisonersTxt.setFillColor(sf::Color::Red);
	m_prisonersTxt.setPosition(TILE_SIZE * m_windowSize.x, 35.f);
	m_prisonersTxt.setStyle(sf::Text::Regular);
	m_prisonersTxt.setString("0 / 0 Prisoners");

	m_panel.setTexture(GameData::getInstance().uiPanelTexture);
	m_panel.setPosition(TILE_SIZE * 0.5f * m_windowSize.x, 5);
}

void GameUI::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_health);
	target.draw(m_power);
	target.draw(m_panel);
	target.draw(m_levelTxt);
	target.draw(m_prisonersTxt);
}

bool GameUI::changeHealth(float change) {
	return m_health.changeHealth(change);
}

bool GameUI::changePower(float change) {
	return m_power.changeHealth(change);
}

float GameUI::getPower() const {
	return m_power.getHealth();
}

float GameUI::getMaxPower() const {
	return m_power.getMaxHealth();
}

void GameUI::reset(int level) {
	m_levelTxt.setString("Level: " + std::to_string(level + 1));
	m_health.reset();
	m_power.reset();
}

void GameUI::setPrisonerInfo(int remaining, int max) {
	m_prisonersTxt.setFillColor(sf::Color::Red);
	m_prisonersTxt.setString(std::to_string(remaining) + " / " + std::to_string(max) + " Prisoners.");
	bool lessThan10PercentPrisoners = ((float)remaining / max) <= 0.1f;
	if (lessThan10PercentPrisoners)
		m_prisonersTxt.setFillColor(sf::Color(20,160,20));
}
