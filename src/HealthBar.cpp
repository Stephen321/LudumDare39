#include "HealthBar.h"

HealthBar::HealthBar() {
}

HealthBar::HealthBar(float maxHealth, const sf::Color& backgroundOutline, const sf::Color& background, const sf::Color& foreground) :
	  m_maxHealth(maxHealth)
	, m_currentHealth(m_maxHealth)
	, m_yOffset(0.f) {
	float height = 24, width = 128;
	m_backgroundRect = sf::RectangleShape(sf::Vector2f(width, height));
	m_backgroundRect.setOutlineColor(backgroundOutline);
	m_backgroundRect.setFillColor(background);
	m_backgroundRect.setPosition(sf::Vector2f(0.f, -1000.f));
	m_backgroundRect.setOutlineThickness(1.f);

	m_healthRect = sf::RectangleShape(sf::Vector2f(width, height));
	m_healthRect.setOutlineColor(sf::Color::Transparent);
	m_healthRect.setFillColor(foreground);
	m_healthRect.setPosition(sf::Vector2f(0.f, -1000.f));
	m_healthRect.setOutlineThickness(1.f);
}

void HealthBar::setPosition(const sf::Vector2f& position) {
	m_backgroundRect.setPosition(position + sf::Vector2f(0.f, m_yOffset));
	m_healthRect.setPosition(position + sf::Vector2f(0.f, m_yOffset));
}

bool HealthBar::changeHealth(float change) {
	bool alive = true;
	if (m_currentHealth + change <= 0) {
		m_currentHealth = 0;
		alive = false;
	}
	else if (m_currentHealth + change > m_maxHealth)
		m_currentHealth = m_maxHealth;
	else
		m_currentHealth += change;
	m_healthRect.setSize(sf::Vector2f((m_currentHealth / m_maxHealth) * m_backgroundRect.getSize().x, m_backgroundRect.getSize().y));
	return alive;
}

float HealthBar::getHealth() const {
	return m_currentHealth;
}

float HealthBar::getMaxHealth() const {
	return m_maxHealth;
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_backgroundRect);
	target.draw(m_healthRect);
}

void HealthBar::reset() {
	m_currentHealth = m_maxHealth;
	m_healthRect.setSize(sf::Vector2f(m_backgroundRect.getSize().x, m_backgroundRect.getSize().y));
}

void HealthBar::setYOffset(float yOffset) {
	m_yOffset = yOffset;
}

void HealthBar::setXSize(float xSize) {
	m_backgroundRect.setSize(sf::Vector2f(xSize, m_backgroundRect.getSize().y));
	m_healthRect.setSize(sf::Vector2f(xSize, m_backgroundRect.getSize().y));
}

void HealthBar::setYSize(float ySize) {
	m_backgroundRect.setSize(sf::Vector2f(m_backgroundRect.getSize().x, ySize));
	m_healthRect.setSize(sf::Vector2f(m_backgroundRect.getSize().x, ySize));
}