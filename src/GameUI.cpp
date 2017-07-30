#include "GameUI.h"
#include "Constants.h"
#include "GameData.h"

GameUI::GameUI(const sf::Vector2u windowSize) :
	  m_windowSize(windowSize)
	, m_powerLevel(3) //TODO: use enum for power level in prisonmanager class
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

	m_powerOverlay.setTexture(GameData::getInstance().powerOverlayTex);
	m_powerOverlay.setPosition(m_windowSize.x * PowerXPos, 5.f);

	m_highPwrOverlay.setTexture(GameData::getInstance().highPwrOverlayTex);
	m_highPwrOverlay.setPosition(m_windowSize.x * PowerXPos, 5.f);
	m_mediumPwrOverlay.setTexture(GameData::getInstance().mediumPwrOverlayTex);
	m_mediumPwrOverlay.setPosition(m_windowSize.x * PowerXPos, 5.f);
	m_lowPwrOverlay.setTexture(GameData::getInstance().lowPwrOverlayTex);
	m_lowPwrOverlay.setPosition(m_windowSize.x * PowerXPos, 5.f);
	m_noPwrOverlay.setTexture(GameData::getInstance().noPwrOverlayTex);
	m_noPwrOverlay.setPosition(m_windowSize.x * PowerXPos, 5.f);


	m_panel.setTexture(GameData::getInstance().uiPanelTexture);
	m_panel.setPosition(TILE_SIZE * 0.5f * m_windowSize.x, 5);


	m_powerIcon.setTexture(GameData::getInstance().powerIconTex);
	m_powerIcon.setPosition((HealthXSize * windowSize.x * 0.5f) + (m_windowSize.x * PowerXPos), 6.f);
	m_healthIcon.setTexture(GameData::getInstance().healthIconTex);
	m_healthIcon.setPosition((HealthXSize * windowSize.x * 0.5f) + (m_windowSize.x * HealthXPos), 6.f);
}

void GameUI::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_power);
	target.draw(m_powerOverlay);
	if (m_powerLevel <= 2)
		target.draw(m_highPwrOverlay);
	if (m_powerLevel <= 1)
		target.draw(m_mediumPwrOverlay);
	if (m_powerLevel <= 0)
		target.draw(m_lowPwrOverlay);
	if (m_powerLevel <= -1)
		target.draw(m_noPwrOverlay);

	target.draw(m_powerIcon); 
	target.draw(m_health);
	target.draw(m_healthIcon);
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
	m_powerLevel = 3;
	m_levelTxt.setString("Level: " + std::to_string(level + 1));
	m_power.reset();
}

void GameUI::setPrisonerInfo(int remaining, int max) {
	m_prisonersTxt.setFillColor(sf::Color::Red);
	m_prisonersTxt.setString(std::to_string(remaining) + " / " + std::to_string(max) + " Prisoners.");
	
	if (remaining == 0)
		m_prisonersTxt.setFillColor(sf::Color(20,160,20));
}

void GameUI::decreasePowerLevel() {
	m_powerLevel--;
}
